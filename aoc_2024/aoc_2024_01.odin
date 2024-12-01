package main

import "core:fmt"
import "core:os"
import "core:strings"
import "core:strconv"
import "core:slice"

Input :: struct {
	left: [dynamic]int,
	right: [dynamic]int,
}

read :: proc() -> Input {
	data, ok := os.read_entire_file_from_handle(os.stdin)
	//data, ok := os.read_entire_file(`in.txt`)
	if !ok {
		fmt.println("Could not read in.txt")
	}
	defer delete(data, context.allocator)
	input : Input
	data_as_string := string(data)
	for line in strings.split_lines_iterator(&data_as_string) {
		numbers := strings.split(line, `   `)
		assert(len(numbers) == 2)
		append(&input.left, strconv.atoi(numbers[0]))
		append(&input.right, strconv.atoi(numbers[1]))
	}
	return input
}

solve1 :: proc(input : Input) {
	slice.sort(input.left[:])
	slice.sort(input.right[:])
	sum := 0
	for i in 0 ..< len(input.left) {
		sum += abs(input.left[i] - input.right[i])
	}
	fmt.println(sum)
}

solve2 :: proc(input : Input) {
	sum := 0
	for i in 0 ..< len(input.left) {
		sum += input.left[i] * slice.count(input.right[:], input.left[i])
	}
	fmt.println(sum)
}

main :: proc() {
	input := read()
	solve1(input)
	solve2(input)
}
