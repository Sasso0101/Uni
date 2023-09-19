use std::io::stdin;

fn factorial(n: i32) -> i32 {
    if n == 1 {
        return 1;
    } else {
        return n*factorial(n-1);
    }
}

pub fn main() {
    let mut user_input = String::new();
    stdin().read_line(&mut user_input).unwrap();
    let user_input = user_input.trim();
    let n = user_input.parse::<i32>().unwrap();
    println!("Factorial of {} is {}! :)", user_input, factorial(n));
}