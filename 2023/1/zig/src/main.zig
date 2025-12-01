const std = @import("std");
const data = @embedFile("input.txt");

pub fn main() !void {
    // var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    // const allocator = gpa.allocator();
    // defer _ = gpa.deinit();
    
    // var sum = 0;
    var splits = std.mem.split(u8, data, "\n");
    while (splits.next()) |line| {
        
        while(line) |c| {
            std.debug.print("{s}",.{c});
        }
    }
}

