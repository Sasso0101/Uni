#![allow(dead_code)]
#![allow(unused)]

use class7::{singlemap, twomaps, lazymap_collect, lazymap_nocollect, string_tolower, maps_options, mapsownership};

mod class1;
mod class2;
mod class3;
mod class4;
mod class7;

pub fn main(){
    foo("ciao".to_string());
  }
  fn foo(str:String) {
    let x = str; 
    let y = &x;
    let z = x;
    let w = &z;
    let t = &z;
    println!("{}", t);
    println!("{}", w);
    // HERE
}

// pub fn main() {
//     // mapsownership();
// }