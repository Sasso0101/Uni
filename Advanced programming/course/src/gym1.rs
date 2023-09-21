use std::io::stdin;

fn string_reverse(input: &str) -> String {
    let mut reversed = String::new();
    for c in input.chars().rev() {
        reversed.push(c);
    }
    reversed
}

fn bigger(n1: i32, n2:i32) -> i32 {
    return if n1 > n2 {n1} else {n2};
}

pub fn main() {
    // let mut user_input = String::new();
    // stdin().read_line(&mut user_input).unwrap();
    // println!("{}", string_reverse(&(user_input.trim())));

    println!("{}", bigger(3, 5));
}