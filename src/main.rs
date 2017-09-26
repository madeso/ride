#[cfg(feature = "backend_gtk")]
mod main_gtk;

#[cfg(feature = "backend_gtk")]
fn main() {
  main_gtk::main();
}

#[cfg(not(feature = "backend_gtk"))]
fn main() {
  // todo: error, not a valid configuration
  println!("No backend configured!");
}
