use std::{thread, time::Duration, fs::File, io::{self, BufRead, BufReader}, num::ParseIntError};

fn read_cpu_times() -> io::Result<(i64, i64)> {
    let file = File::open("/proc/stat")?;
    let mut reader = BufReader::new(file);
    let mut line = String::new();

    if reader.read_line(&mut line)? > 0 {
        let parts: Vec<&str> = line.split_whitespace().collect();
        if parts.len() < 5 {
            return Err(io::Error::new(io::ErrorKind::Other, "Unexpected /proc/stat format"));
        }
        let total_time: i64 = parts.iter().skip(1).take(8) // Take 8 to include guest and guest_nice
            .filter_map(|x| x.parse::<i64>().ok()).sum();
        let idle_time: i64 = parts[4].parse().map_err(|e: ParseIntError| io::Error::new(io::ErrorKind::Other, e.to_string()))?;
        Ok((total_time, idle_time))
    } else {
        Err(io::Error::new(io::ErrorKind::Other, "Unable to read /proc/stat"))
    }
}

fn main() -> io::Result<()> {
    let (total_time1, idle_time1) = read_cpu_times()?;
    thread::sleep(Duration::from_millis(500)); // Sleep for 100 milliseconds
    let (total_time2, idle_time2) = read_cpu_times()?;

    let total_diff = total_time2 - total_time1;
    let idle_diff = idle_time2 - idle_time1;

    let cpu_usage = 100.0 * (total_diff - idle_diff) as f64 / total_diff as f64;

    println!("CPU Usage: {:.2}%", cpu_usage);

    Ok(())
}
