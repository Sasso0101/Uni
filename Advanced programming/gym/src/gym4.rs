fn find_equal<'a>(s1: &'a str, s2: &'a str) -> Option<(&'a str, &'a str)> {
    for i in 1..(s1.len()-1) {
        if s1[i-1..=i] == s2[i-1..=i] {
            return Some((&s1[i-1..=i], &s2[i-1..=i]))
        }
    }
    None
}

use rand::{distributions::Standard, Rng};
fn lucky_slice(input_str: &str) -> String {
    let mut rand::thread_rng();
    
}

pub fn main() {
    // println!("{:?}", find_equal("ciao", "tias").unwrap());
    let s = lucky_slice("ciao");
    println!("{}", s);
    println!("{:?}", find_equal(&s, "ciao"));
}