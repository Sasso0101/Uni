//use core::fmt;
use std::collections::HashMap;

fn _is_it_lehn(code: &str) -> bool {
    let code = code.replace(" ", "");
    let sum: u32 = code.chars().rev().enumerate().fold(0, |acc, (i, n)| {
        let mut n = n.to_digit(10).unwrap();
        if i % 2 != 0 { 
            n *= 2;
            if n > 9 { n -= 9}
        }
        acc + n
    });
    sum % 10 == 0
}

enum _CarType {
    Diesel,
    Gasoline,
    LPG,
    Methane,
    Electric
}

#[derive(Debug)]
enum _IP {
    _V4(u8, u8, u8, u8),
    _V6(u32, u32, u32, u32)
}

struct _Point {
    x: i32,
    y: i32,
    z: i32
}

fn _recognise_owner<'a>(map:&'a HashMap<String, String>, plate:&String ) -> Option<&'a String> {
    map.get(plate)
}
/*/
#[derive(Eq, Hash, PartialEq)]
enum Item {
    Coke,
    Coffee,
    Snack,
    IceCream
}

enum Coin {
    Cents10,
    Cents20,
    Cents50,
    Euro1,
    Euro2
}

impl Coin {
    fn to_cents(&self) -> u32 {
        match self {
            Coin::Cents10 => {10}
            Coin::Cents20 => {20}
            Coin::Cents50 => {50}
            Coin::Euro1 => {100}
            Coin::Euro2 => {200}
        }
    }
}

struct VendingMachine {
    coins: u32,
    items: HashMap<Item, (usize, u32)>
}

impl VendingMachine {
    fn new(items: HashMap<Item, (usize, u32)>) -> Self {
        Self {
            coins: 0,
            items
        }
    }

    fn add_item(&mut self, item: Item, quantity: usize, price: u32) {
        self.items.insert(item, (quantity, price));
    }

    fn insert_coin(&mut self, coin: Coin) {
        self.coins += coin.to_cents();
    }

    fn buy(&mut self, item: &Item) -> Result<u32, String> {
        match self.items.get_mut(item) {
            Some((quantity, price)) => {
                if *quantity == 0 {
                    Err("Out of stock!".to_string())
                } else if self.coins < *price {
                    Err("More coins needed!".to_string())
                } else {
                    *quantity -= 1;
                    let change = self.coins-*price;
                    self.coins = 0;
                    Ok(change)
                }
            },
            None => {
                Err("Item does not exist!".to_string())
            }
        }
    }

}
*/

/*/
#[derive(Debug)]
struct Date(u8, u8, u16);
#[derive(Debug)]
struct Hour(u8, u8);
#[derive(Debug)]
struct BoxShipping {
    name: String,
    barcode: String,
    shipment_date: Date,
    shipment_time: Hour
}

impl fmt::Display for Date {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}/{}/{}", self.0, self.1, self.2)
    }
}
impl fmt::Display for Hour {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}:{}", self.0, self.1)
    }
}

impl fmt::Display for BoxShipping {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "Name: {}\nBarcode: {}\nShipment date: {}\nShipment time: {}",
            self.name, self.barcode, self.shipment_date, self.shipment_time)
    }
}
*/

pub fn main() {
    // println!("{}", is_it_lehn("4539 3195 0343 6467"));
    // println!("{:#?}", IP::V4(0, 0, 0, 0));

    // let mut map:HashMap<String, String> = HashMap::new();
    // map.insert("aaa".to_string(), "tizio".to_string());
    // map.insert("bbb".to_string(), "caio".to_string());

    // println!("{:?}", recognise_owner(&map, &"aaa".to_string()));

    /*
    let mut map = HashMap::new();
    map.insert(Item::Coke, (10, 200));
    map.insert(Item::Coffee, (10, 100));
    map.insert(Item::IceCream, (10, 200));
    map.insert(Item::Snack, (10, 200));

    let mut machine = VendingMachine::new(map);
    machine.insert_coin(Coin::Cents50);
    machine.insert_coin(Coin::Cents50);
    machine.insert_coin(Coin::Cents50);
    println!("Change: {}cents", machine.buy(&Item::Coffee).unwrap());
    */

    /*let b0 = BoxShipping {
        name: "test".to_string(),
        barcode: "123456".to_string(),
        shipment_date: Date(5,10,2023),
        shipment_time: Hour(10,12)
    };
    let b1 = BoxShipping {
        name: "test".to_string(),
        barcode: "123456".to_string(),
        shipment_date: Date(6,10,2023),
        shipment_time: Hour(10,12)
    };
    println!("{:?}\n{}", b0, b1);*/
}