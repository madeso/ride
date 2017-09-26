#[cfg(feature = "backend_gtk")]
mod main_gtk;

#[cfg(feature = "backend_win32")]
mod main_win32;

#[cfg(feature = "backend_gtk")]
fn main() {
  main_gtk::main();
}

#[cfg(feature = "backend_win32")]
fn main() {
  main_win32::main();
}

#[cfg(not(any(feature = "backend_gtk", feature = "backend_win32")))]
fn main() {
  // todo: error, not a valid configuration
  println!("No backend configured!");
}
