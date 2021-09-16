namespace {
  // before struct
  pub struct MyStruct {
    member : i32 // member line
  } // last struct line

  // before impl MyStruct
  impl MyStruct {} // newline between { and }
  // some line


  // before impl MyStruct
  impl MyStruct {} // newline after }
  // some line
}

fn main() {
    let c = Circle { x: 0.0, y: 0.0, radius: 2.0 };
    println!("{}", c.area());
}