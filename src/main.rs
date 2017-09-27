#[cfg(feature = "backend_gtk")]
mod main_gtk;

#[cfg(feature = "backend_win32")]
mod main_win32;

#[cfg(feature = "backend_gtk")]
fn launch_application(title : &str) {
  main_gtk::launch_application(title);
}

#[cfg(feature = "backend_win32")]
fn launch_application(title : &str) {
  main_win32::launch_application(title);
}

#[cfg(not(any(feature = "backend_gtk", feature = "backend_win32")))]
fn launch_application(title : &str) {
  // todo: error, not a valid configuration
  println!("No backend configured for {}", title);
}

fn main() {
  // todo: error, not a valid configuration
  launch_application("Ride - Rust IDE")
}
