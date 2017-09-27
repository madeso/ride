extern crate gtk;
extern crate cairo;
extern crate gdk;

use std::f64::consts::PI;

use self::gtk::prelude::*;
use self::gtk::{Button, Window, WindowType};
use self::gtk::DrawingArea;

use self::cairo::enums::{FontSlant, FontWeight};
use self::cairo::Context;

use Application;
use Drawer;

impl Drawer for Context {
  fn text(&self, text : &str, x: i32, y: i32)
  {
    self.move_to(x as f64, y as f64);
    self.show_text(text);
  }
}

fn print_key(key : &gdk::EventKey) -> Option<char> {
  return gdk::keyval_to_unicode(key.get_keyval())
}

pub fn launch_application(width: i32, height: i32, title : &str, app : Application) {
    if gtk::init().is_err() {
        println!("Failed to initialize GTK.");
        return;
    }

    let window = Window::new(WindowType::Toplevel);
    window.set_title(title);
    window.set_default_size(width, height);

    let drawing_area = Box::new(DrawingArea::new)();
    window.add(&drawing_area);
    window.connect_key_press_event(|_, key| {
      println!("The key pressed was {:?}.", print_key(key));
      Inhibit(true)
    });
    window.connect_key_release_event(|_, key| {
      println!("The key released was {:?}.", print_key(key));
      Inhibit(true)
    });
    window.connect_button_press_event(|_, key| {
      // works = mouse
      println!("The mouse button pressed was {:?}.", key.get_button());
      Inhibit(true)
    });
    window.connect_button_release_event(|_, key| {
      // works = mouse
      println!("The mouse button released was {:?}.", key.get_button());
      Inhibit(true)
    });

    drawing_area.connect_draw(move |_, cr| {
        // cr.scale(500f64, 500f64);

        cr.select_font_face("Sans", FontSlant::Normal, FontWeight::Normal);
        cr.set_font_size(12f64);

        app.draw(cr);

        Inhibit(false)
    });

    window.connect_delete_event(|_, _| {
        gtk::main_quit();
        Inhibit(false)
    });

    window.show_all();
    gtk::main();
}
