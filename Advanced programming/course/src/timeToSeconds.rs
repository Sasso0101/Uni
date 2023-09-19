use std::io::stdin;

fn split_to_time(input: String) -> [i32; 3] {
    let iter = input.split(':');
    let mut timestamp: [i32; 3] = [0; 3];
    
    for (i, time) in iter.enumerate() {
        timestamp[i] = time.trim().parse::<i32>().unwrap();
    }
    
    timestamp
}

pub fn main() {
    // Read timestamp, return number of seconds
    let stdin = stdin();
    let mut user_input = String::new();
    stdin.read_line(&mut user_input).unwrap();
    let time = split_to_time(user_input);
    let mut total = 0;
    total += time[0]*60*60;
    total += time[1]*60;
    total += time[2];
    println!("Timestamp: {}:{}:{}", time[0], time[1], time[2]);
    println!("Total seconds: {}", total);
}