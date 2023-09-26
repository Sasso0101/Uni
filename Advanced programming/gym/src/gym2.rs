use std::collections::HashMap;

fn _modify_odd(arr: &mut [i32]) {
    for slice in arr {
        if *slice % 2 != 0 {*slice = 0;}
    }
}

fn _generate_vec_odd() {
    let mut vect: Vec<i32> = Vec::with_capacity(100);
    for i in 1..100 {
        vect.push(i as i32);
    }
    _modify_odd(vect.as_mut());
    println!("{:?}", vect);
}

fn _count_character(input: String) -> HashMap<char, u32> {
    let mut map: HashMap<char, u32> = HashMap::new();
    for c in input.chars() {
        map.entry(c).and_modify(|e| *e+= 1).or_insert(1);
    }
    map
}

fn _split_at_slice(sl: &[i32], v: i32) -> Option<(&[i32], &[i32])> {
    let i = sl.iter().position(|e| *e == v);
    i.and_then(|pos| Some(sl.split_at(pos)))
}

fn _sub_slice(v1: &Vec<i32>, v2: &Vec<i32>) {
    let cointained = v2.iter().all(|item| v1.contains(item));
    if cointained {println!("{:?}", v2);}
    else {println!("Not found");}
}

fn _max(v: &Vec<i32>) -> i32 {
    *v.iter().max().unwrap()
}

fn _swap_rust(v: &mut Vec<i32>) {
    let len = v.len();
    v.swap(0, len-1);
}

fn _swap_custom(v: &mut Vec<i32>) {
    let (first_p, v) = v.split_first_mut().unwrap();
    let (last_p, ..) = v.split_last_mut().unwrap();
    let temp = *first_p;
    *first_p = *last_p;
    *last_p = temp;
}

fn _is_sorted(v: &Vec<i32>) -> bool {
    v.iter().enumerate().all(|(i, val)| {
        if i != 0 {val >= &v[i-1]}
        else {true}
    })
}

fn _insert_if_longer(mut v: Vec<String>, s: String) -> Vec<String> {
    if s.len() > 10 {
        v.push(s);
    }
    v
}

fn _build_vector(it: core::slice::Iter<i32>) -> Vec<&i32> {
    it.collect()
}

// Flip elements up to element at index n
fn _flip(v: &mut Vec<i32>, n: usize) {
    if n == 0 {return}
    for i in 0..n/2 {
        v.swap(i, n-i)
    }
}

fn _pancake_sorting(v: &mut Vec<i32>) {
    let mut flip_len = v.len()-1;
    while flip_len != 0 {
        let (swap_pos, ..) = v[..=flip_len].iter().enumerate().max_by(|(.., n1), (.., n2)| n1.cmp(n2)).unwrap();
        if swap_pos != flip_len {
            if swap_pos != 0 {_flip(v, swap_pos);}
            _flip(v, flip_len);
        }
        flip_len -= 1;
    }
}

fn _merge(s1: &[i32], s2: &[i32]) -> Vec<i32> {
    let mut a = [s1, s2].concat();
    a.sort();
    a.into()
}

#[derive(Debug)]
#[allow(dead_code)]
enum _Alfanumeric{
    String(String),
    Number(i32)
}

fn _alfanumeric() {
    let mut v: Vec<_Alfanumeric> = Vec::new();
    v.push(_Alfanumeric::Number(3));
    v.push(_Alfanumeric::String("hello".to_string()));
    println!("{:?}", v);
}

enum _Operation {
    Add,
    Sub,
    Mul,
    Div
}

enum _Expression {
    Number(i32),
    Operation (Box<_Expression>, _Operation, Box<_Expression>)
}

fn _evaluate_expression(e: _Expression) -> Result<i32, String> {
    match e {
        _Expression::Operation(ex1, op, ex2) => {
            let ex1 = _evaluate_expression(*ex1)?;
            let ex2 = _evaluate_expression(*ex2)?;
            match op {
                _Operation::Add => {
                    Ok(ex1 + ex2)
                },
                _Operation::Sub => {
                    Ok(ex1 - ex2)
                },
                _Operation::Mul => {
                    Ok(ex1 * ex2)
                },
                _Operation::Div => {
                    Ok(ex1 / ex2)
                }
            }
        },
        _Expression::Number(n) => {Ok(n)}
    }
}

pub fn main() {
    // let mut arr = [5, 7, 8, 10, 12, 3, 6, 8, 2, 3, 5];
    // modify_odd(arr.as_mut());
    // println!("{:?}", arr);

    // generate_vec_odd();

    // let s = String::from("lorem ipsum dolor sit amet");
    // println!("{:?}", count_character(s));

    // let arr = [5, 7, 8, 10, 12, 3, 6, 8, 2, 3, 5];
    // match split_at_slice(&arr, 17) {
    //     Some(t) => {
    //         println!("{:?}", t);
    //     }
    //     None => {
    //         println!("Element not present in list!");
    //     }
    // }

    // let v1 = vec![1, 6, 8, 13, 24, 5, 9, 8, 6];
    // let v2 = vec![6, 13, 6, 12];
    // sub_slice(&v1, &v2);

    // let mut v = vec![1, 6, 8, 13, 24, 5, 9, 8, 6];
    
    // println!("{}", max(&v));
    
    // println!("Before: {:?}", v);
    // swap_rust(&mut v);
    // println!("After: {:?}", v);
    // swap_custom(&mut v);
    // println!("After: {:?}", v);
    
    // let ordered = vec![-4, 6, 7, 6, 10];
    // println!("{:?}", is_sorted(&ordered));

    // let words = vec!["lorem".to_string(), "ipsum".to_string(), "dolor".to_string(), "sic".to_string(), "amet".to_string()];
    // println!("{:?}", insert_if_longer(words, "not10lett1".to_string()))

    // let it = v.iter();
    // println!("{:?}", build_vector(it));

    // println!("{:?}", v);
    // flip(&mut v, 6);
    // println!("{:?}", v);

    // println!("Unsorted: {:?}", v);
    // pancake_sorting(&mut v);
    // println!("Sorted: {:?}", v);

    // let s1 = [8, 4, 2, 12, 19, 4, 5];
    // let s2 = [5, 8, 9, 2 , 4, 5];
    // println!("{:?}", merge(&s1, &s2));

    // alfanumeric();

    // let n1 = Box::new(Expression::Number(10));
    // let n2 = Box::new(Expression::Number(7));
    // let exp1 = Box::new(Expression::Operation(n1, Operation::Mul, n2));
    // let n3 = Box::new(Expression::Number(3));
    // let exp2 = Expression::Operation(exp1, Operation::Sub, n3);
    // (10*7)-3
    // println!("{:?}", evaluate_expression(exp2));
}