#[cfg(feature = "backend_gtk")]
mod main_gtk;

#[cfg(feature = "backend_win32")]
mod main_win32;

pub trait Drawer {
  fn text(&self, text : &str, x: i32, y: i32);
}

pub trait Backend {
  fn redraw(&self);
}

pub struct Application {
  text : String
}

impl Application {
  fn draw(&self, draw : &Drawer)
  {
    draw.text(&self.text.to_string(), 20, 20);
  }

  fn on_char(&mut self, ch : char, backend: &Backend)
  {
    self.text.push(ch);
    backend.redraw();
    // println!("Char recieved {}", ch);
  }
}

#[cfg(feature = "backend_gtk")]
fn launch_application(width: i32, height: i32, title : &str, app: Application) {
  main_gtk::launch_application(width, height, title, app);
}

#[cfg(feature = "backend_win32")]
fn launch_application(width: i32, height: i32, title : &str, app: Application) {
  main_win32::launch_application(title);
}

#[cfg(not(any(feature = "backend_gtk", feature = "backend_win32")))]
fn launch_application(width: i32, height: i32, title : &str, app: Application) {
  // todo: error, not a valid configuration
  println!("No backend configured for {}", title);
}

fn main() {
  // todo: error, not a valid configuration
  launch_application(800, 600, "Ride - Rust IDE", Application{text: String::from("")})
}
