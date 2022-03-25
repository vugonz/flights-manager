from random import randint as r
from random import shuffle
from itertools import permutations
#if you are havicg time limit exceeded errors you can run this script to get an extensive test to use as a reference
#before acd after some changes

#each test is racdom and the time it will take to run is also machine dependant, simply knowing how fast your program runs
#this test before acd after you do changes is just a brute observation, this is by no means specific
def main():
    letters = ["A", "B", "C", "D", "E"]
    airports = list(permutations(letters, 3))[:40] 
    airports = [''.join(i) for i in airports]
    shuffle(airports)


    with open("extensive_test.in", "w") as f:
        for airport in airports:
            print(f"a {airport} Country City", file=f)

    c = 0
    add_flight(airports[1], airports[2], c, 'A')
    add_flight(airports[2], airports[3], c, 'B')
    add_flight(airports[3], airports[1], c, 'C')
    add_flight(airports[4], airports[8], c, 'D')
    add_flight(airports[5], airports[14], c, 'E')
    add_flight(airports[6], airports[13], c, 'F')
    add_flight(airports[7], airports[14], c, 'G')
    add_flight(airports[8], airports[14], c, 'H')
    add_flight(airports[9], airports[13], c, 'I')
    add_flight(airports[10], airports[12], c, 'J')
    add_flight(airports[11], airports[10], c, 'K')
    add_flight(airports[9], airports[8], c, 'L')
    add_flight(airports[2], airports[6], c, 'M')
    add_flight(airports[3], airports[5], c, 'N')
    add_flight(airports[8], airports[4], c, 'O')
    add_flight(airports[2], airports[1], c, 'P')
    add_flight(airports[3], airports[3], c, 'Q')
    add_flight(airports[8], airports[2], c, 'R')
    add_flight(airports[1], airports[2], c, 'S')
    add_flight(airports[14], airports[4], c, 'T')
    add_flight(airports[10], airports[11], c, 'U')
    add_flight(airports[2], airports[12], c, 'V')
    add_flight(airports[9], airports[3], c, 'X')
    add_flight(airports[14], airports[15], c, 'Y')
    add_flight(airports[4], airports[2], c, 'Z')

def add_flight(origin, destination, c, letter):
    n = 1
    with open("extensive_test.in", "a") as f:
        while n < 1150:
            day = r(1, 30)
            month = r(1, 12)
            min = r(0, 59)
            hour = r(0,23)
            dur_min = r(0, 59)
            dur_hour = r(0, 11)
            print(f"v A{letter}{750*c + n} {origin} {destination} {day:02}-{month:02}-2022 {hour:02}:{min:02} {dur_hour:02}:{dur_min:02} 50", file=f)
            n += 1

    return;

if __name__ == '__main__':
    main()
