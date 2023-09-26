// use std::io::stdin;

use std::collections::HashMap;

fn _string_reverse(input: &str) -> String {
    let mut reversed = String::new();
    for c in input.chars().rev() {
        reversed.push(c);
    }
    reversed
}

fn _bigger(n1: i32, n2:i32) -> i32 {
    return if n1 > n2 {n1} else {n2};
}

fn _multiply(n1: i32, n2: f32, n3: f64) -> f64 {
    n3 * n2 as f64 * n1 as f64
}

const _LIGHT_IN_VACUUM:i32 = 299792458;

fn _e_equals_mc_squared(mass: f32) -> f32 {
    (mass*_LIGHT_IN_VACUUM as f32).powi(2)
}

fn _max_min(numbers: &Vec<i32>) -> (i32, i32) {
    let mut min:i32 = *numbers.get(0).unwrap();
    let mut max:i32 = *numbers.get(0).unwrap();
    for n in numbers {
        if *n < min {min = *n};
        if *n > max {max = *n};
    }
    return (max, min)
}

fn _lord_farquaad(s: String) -> String {
    s.replace("e", "💥")
}

fn _extract_price(furniture_map: HashMap<String, f32>, furniture: String) -> f32 {
    match furniture_map.get(&furniture) {
        Some(value) => {
            *value
        }
        None => {
            -1.0
        }
    }
}

fn _append(s: &String) -> String {
    let mut new_string = s.clone();
    new_string.push_str("foobar");
    new_string
}

fn _is_armstrong(input: i32) -> bool {
    let mut digits:Vec<i32> = vec![];
    let mut n = input;
    while n != 0 {
        digits.push(n % 10);
        n = n / 10;
    }
    let length = digits.len() as u32;
    let mut sum = 0;
    for i in digits {
        sum += i.pow(length);
    }
    if sum == input {true}
    else {false}
}

fn _is_armstrong_string(n: i32) -> bool {
    let input = n.to_string();
    let len = input.len();
    let mut sum = 0;
    for char in input.chars() {
        sum += (char.to_digit(10).unwrap()).pow(len as u32);
    }
    sum as i32 == n
}

pub fn _transpose(mut matrix: ((i32, i32), (i32, i32))) -> ((i32, i32), (i32, i32)) {
    let temp = matrix.0.1;
    matrix.0.1 = matrix.1.0;
    matrix.1.0 = temp;
    matrix
}

pub fn main() {
    // let mut user_input = String::new();
    // stdin().read_line(&mut user_input).unwrap();
    // println!("{}", string_reverse(&(user_input.trim())));
    
    // println!("{}", bigger(3, 5));
    
    // println!("{}", e_equals_mc_squared(3.32));

    // let numbers = vec![4, 7, 9, 8];
    // let (max, min) = max_min(&numbers);
    // println!("Vector: {:?}\nMin: {}, max: {}", numbers, min, max);

    //println!("{}", lord_farquaad(String::from("lorem ipsum dolor sit amet")));

    // let furniture:HashMap<String, f32> = HashMap::from([
    //     (String::from("Table"), 58.98),
    //     (String::from("Chair"), 17.92),
    //     (String::from("Bed"), 321.90)
    // ]);
    // let price = extract_price(furniture, String::from("Chair"));
    // println!("Price: {}", price);

    // let s = String::from("hello");
    // let new_string = append(&s);
    // println!("Old string: {}, new string: {}", s, new_string);

    // let n = 154;
    // println!("n: {}, armstrong: {}", n, is_armstrong(n));

    // let matrix = (
    //     (1, 2),
    //     (3, 4)
    // );
    // println!("Initial matrix: {:?}\nTransposed matrix: {:?}", matrix, transpose(matrix));
}