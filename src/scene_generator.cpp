/**
MIT License

Copyright (c) 2019 Alexandr Krikun

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
 */

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

int main (int argc, char **argv)
{
  // -f filename -n number -t delta_time --stdout

  char *filename = NULL;
  int cnt = -1;
  bool drop_on_stdout = false;
  double delta_time = 1;
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] == '-')
      switch(argv[i][1]) {
        case 'f':
          filename = argv[i + 1];
          break;
        case 'n':
          cnt = atoi(argv[i + 1]);
          break;
        case 't': {
          char *end;
          delta_time = strtold(argv[i + 1], &end);
          break;
        }
        case '-':
          if (!strcmp(argv[i] + 2, "stdout"))
            drop_on_stdout = true;
          break;
      };
  }

  if(filename == NULL) {
    std::cerr << "Specify a filename!\n";
    return 1;
  }
  if(cnt == -1) {
    std::cerr << "Specify a number of bodies!\n";
    return 1;
  }
  if (delta_time == 1)
    std::cout << "Delta_time is unspecified and equals 1.\n";

  srand((std::chrono::system_clock::now().time_since_epoch()).count());

  char path[] = "scenes/";
  strcat(path, filename);
  strcat(path, ".simsetup");
  std::fstream model_file(path, std::ios_base::out);
  if (!model_file.good()) {
    std::cerr << "Couldn't open a file!\n";
    return 1;
  }
  model_file << cnt << ' ' << delta_time << ' ' << " 1" << '\n';
  for (int i = 0; i < cnt; i++) 
  {
    unsigned short x = rand() % 1366,
                   y = rand() % 768;
    int denom;
    for (denom = (rand() % 1000); denom == 0; denom = (rand() % 1000));
    float vel_x = (float)((rand() % 100) - 50) / (float)denom;

    for (denom = (rand() % 1000); denom == 0; denom = (rand() % 1000));
    float vel_y = (float)((rand() % 100) - 50) / (float)denom;

    for (denom = (rand() % 10000); denom == 0; denom = (rand() % 10000));
    int mass_chance = rand() % 100;
    double mass;
    if (mass_chance <= 5) // massive
      mass = fabs((rand() * 1E14) / denom);
    else if (mass_chance <= 45) // normal
      mass = fabs((rand() * 1E8) / denom);
    else // light
      mass = fabs((rand() * 1E5) / denom);


    unsigned radius = rand() % 20 + 3;
    unsigned color[3];
    for(int j = 0; j < 3; j++)
      color[j] = rand() % 255;
    if (drop_on_stdout)
      std::cout << x << ' ' << y << ' ' << vel_x << ' ' << vel_y << ' ' << mass << ' '
                << radius << ' ' << color[0] << ' ' << color[1] << ' ' << color[2] << '\n';
    model_file << x << ' ' << y << ' ' << vel_x << ' ' << vel_y << ' ' << mass << ' '
               << radius << ' ' << color[0] << ' ' << color[1] << ' ' << color[2] << '\n';
  }
  
  model_file.close();

  return 0;
}