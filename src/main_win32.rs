extern crate winapi;
extern crate user32;
extern crate kernel32;
extern crate gdi32;

use std::ffi::OsStr;
use std::os::windows::ffi::OsStrExt;
use std::iter::once;
use std::mem;
use std::ptr::null_mut;
use std::io::Error;

use std::mem::{zeroed};
use self::gdi32::{TextOutA};

use self::user32::{
    DefWindowProcW,
    RegisterClassW,
    CreateWindowExW,
    TranslateMessage,
    DispatchMessageW,
    GetMessageW,
};
use self::user32::{PostQuitMessage, BeginPaint};
use self::winapi::{UINT, WPARAM, LPARAM, LRESULT, LPVOID, LPCSTR, LPCWSTR};

use self::winapi::HWND;
use self::kernel32::GetModuleHandleW;

use self::winapi::winuser::{
    MSG,
    WNDCLASSW,
    CS_OWNDC,
    CS_HREDRAW,
    CS_VREDRAW,
    CW_USEDEFAULT,
    WS_OVERLAPPEDWINDOW,
    WS_VISIBLE,
};

fn win32_string( value : &str ) -> Vec<u16> {
    OsStr::new( value ).encode_wide().chain( once( 0 ) ).collect()
}

struct Window {
    handle : HWND,
}

use self::winapi::{SW_SHOWDEFAULT, WM_DESTROY, WM_PAINT};

static SZ_TEXT: &'static [u8] = b"Hello, world!";

unsafe extern "system"
fn wnd_proc(hwnd: HWND, msg: UINT, wparam: WPARAM, lparam: LPARAM) -> LRESULT {
    match msg {
        WM_DESTROY => {
            PostQuitMessage(0); 0
        },
        WM_PAINT => {
            let mut ps = zeroed();
            let hdc = BeginPaint(hwnd, &mut ps);
            TextOutA(hdc, 5, 5,
                SZ_TEXT.as_ptr() as *const i8,
                SZ_TEXT.len() as i32
            );
            0
        },
        _ => {
            DefWindowProcW(hwnd, msg, wparam, lparam)
        }
    }
}

fn create_window( name : &str, title : &str ) -> Result<Window, Error> {
    let name = win32_string( name );
    let title = win32_string( title );

    unsafe {
        let hinstance = GetModuleHandleW( null_mut() );
        let wnd_class = WNDCLASSW {
            style : CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc : Some( wnd_proc ),
            hInstance : hinstance,
            lpszClassName : name.as_ptr(),
            cbClsExtra : 0,
            cbWndExtra : 0,
            hIcon: null_mut(),
            hCursor: null_mut(),
            hbrBackground: null_mut(),
            lpszMenuName: null_mut(),
        };

        RegisterClassW( &wnd_class );

        let handle = CreateWindowExW(
            0,
            name.as_ptr(),
            title.as_ptr(),
            WS_OVERLAPPEDWINDOW | WS_VISIBLE,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            null_mut(),
            null_mut(),
            hinstance,
            null_mut() );

        if handle.is_null() {
            println!("Failed to create window");
            Err( Error::last_os_error() )
        } else {
            Ok( Window { handle } )
        }
    }
}

fn handle_message( window : &mut Window ) -> bool {
    unsafe {
        let mut message : MSG = mem::uninitialized();
        if GetMessageW( &mut message as *mut MSG, window.handle, 0, 0 ) > 0 {
            TranslateMessage( &message as *const MSG );
            DispatchMessageW( &message as *const MSG );
            true
        } else {
            false
        }
    }
}

pub fn launch_application(title : &str) {
    let mut window = create_window( "ride", title).unwrap();

    loop {
        if !handle_message( &mut window ) {
            break;
        }
    }
}
