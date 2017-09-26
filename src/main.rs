#[cfg(feature = "backend_gtk")]
mod gtk_main;

#[cfg(feature = "backend_gtk")]
fn main() {
  gtk_main::main();
}

#[cfg(not(feature = "backend_gtk"))]
fn main() {
  // todo: error, not a valid configuration
  println!("No backend configured!");
}
