#root -l -b -q 'ptHatCutter.C+(30,50,40)'
#root -l -b -q 'ptHatCutter.C+(50,80,40)'
#root -l -b -q 'ptHatCutter.C+(80,120,40)'
#root -l -b -q 'ptHatCutter.C+(120,170,40)'
#root -l -b -q 'ptHatCutter.C+(170,9999,40)'

g++ ptHatCutter.C $(root-config --cflags --libs) -Wall -Wextra -g -o ptHatCutter.exe

input="input.root";
input="output.root";
colli=2;
./ptHatCutter.exe "${input}" "${output}" $colli 30 50

