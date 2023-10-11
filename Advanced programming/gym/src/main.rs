mod gym1;
mod gym2;
mod gym3;
mod midterm1_mock;
mod midterm1_2022_v1;

struct test {
    a: String
}


pub fn main() {
    gym1::main();
    gym2::main();
    gym3::main();
    // midterm1_mock::main();
    // midterm1_2022_v1::main();
    // let mut a = String::from("ciao");
    // let x = &a;
    // println!("{}", x);
    // let c = a;

    let s = String::new();
    let a = &s;
    println!("{} {}", *a, s);
}