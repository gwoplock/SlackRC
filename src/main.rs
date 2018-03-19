
use std::fs::File;
use std::io::prelude::*;
use std::process;

fn main() {
    //read key from file

    let temp = File::open("~/SlackToken");
    let mut file;
    if temp.is_ok() {
        file = temp.ok().unwrap();
    } else {
        print!("Cant open token file, {}", temp.err().unwrap());
        process::exit(1);
    }
    let mut token = String::new();
    let _ = file.read_to_string(&mut token);
    
    //rtm.connect
    //read res from ^
    //open webhook url from res
    //print messages as the come
}
