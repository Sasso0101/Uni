use std::mem::replace;

mod enumx;
mod structx;
mod modfun;

enum XX {
    Y1(i32, i32),
    Y2(String)
}

enum Z {
    Y1(i32, i32),
    Y2(bool, String)
}

struct Balance {
    amt: i32
}

impl Balance {
    fn maybewithdraw(&self, n: i32) -> Option<i32> {
        if self.amt > n {Some(self.amt - n)}
        else {None}
    }
}

fn data(data: &XX) -> i32 {
    match data {
        XX::Y1(n1, n2) => {
            n1*n2
        },
        XX::Y2(s) => {
            s.len() as i32
        }
    }
}

fn maybelength(data: &Z) -> Option<usize> {
    match data {
        Z::Y1(_, _) => {
            None
        },
        Z::Y2(_, s) => {
            Some(s.len())
        }
    }
}

fn maybesqrt(n: i32) -> Option<i32> {
    if n >= 0 {Some((n as f32).sqrt() as i32)}
    else {None}
}

fn prevchar(c: char) -> char {
    if c == 'a' {
        '\''
    } else {
        ((c as u8) - 1) as char
    }
}

fn replwithprev(s: &mut String) -> Result<String, ()> {
    let mut res = String::new();
    for c in s.chars().into_iter() {
        if c != 'a' {
            res.push(prevchar(c));
        } else {
            return Err(())
        }
    }
    Ok(res)
}

struct X {
    s: String,
    i: i32
}

impl X {
    fn new(&self) -> Self {
        Self {
            s: "xxx".to_string(),
            i: 10
        }
    }
    
    fn getstring(&self) -> String {
        let mut s = String::new();
        replace(&mut s, self.s);
        s
    }
}

struct Y {
    z: bool,
    c: String
}

impl Y {
    fn new(&self) -> Self {
        Self {
            z: true,
            c: "op".to_string()
        }
    }
}

pub fn main() {
    let xx1 = XX::Y1(2,3);
    println!("data {}",data(&xx1));
    let xx1 = XX::Y2(String::from("test"));
    println!("data {}",data(&xx1));


    let z1 = Z::Y1(1,2);
    let z2 = Z::Y2(true,String::from("new"));
    println!("len {:?}", maybelength(&z1));
    println!("len {:?}", maybelength(&z2));

    let ex = enumx::X::Y(String::from("test"));
    let sx = structx::X{i:String::from("asd")};
    println!("Longer {}", modfun::longer(&ex,&sx));
    let ex = enumx::X::Y(String::from("asdasd"));
    println!("Longer {}", modfun::longer(&ex,&sx));

    println!("Maybesqrt 4 {:?} ", maybesqrt(4));
    println!("Maybesqrt -4 {:?} ", maybesqrt(-4));

    let b = Balance{amt:100};
    println!("maybewith {:?}", b.maybewithdraw(10));
    println!("maybewith {:?}", b.maybewithdraw(200));

    println!("char {}, prev {}", 'c', prevchar('c'));
    println!("char {}, prev {}", 'a', prevchar('a'));
    println!("char {}, prev {}", 'z', prevchar('z'));
    let mut s = String::from("Pign");
    println!("S {}",s);
    let ret = replwithprev(&mut s);
    println!("Returned: {:?}",ret);
    let mut s = String::from("pigna");
    println!("S {}",s);
    let ret = replwithprev(&mut s);
    println!("Returned: {:?}",ret);
}