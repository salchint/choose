// Copyright (C) 2017 Thomas Strunz
//
// This file is part of choose.
//
// choose is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// choose is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with choose.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <chrono>

bool drawFromBinaryDistribution()
{
  auto ts = std::chrono::system_clock::now();
//  std::cout << "TS: " << ts.time_since_epoch().count() << std::endl;
  std::cout << "RAN: " << (ts.time_since_epoch().count() %2 > 0) <<  std::endl;

  return ts.time_since_epoch().count() %2 > 0;
}

int main(int /*argc*/, char **)
{
  std::cout << "choose - random path demo                              Thomas Strunz (c) 2017 " << std::endl;

  int intensity = 0;

  for (int i=0; i<100000; ++i)
  {
    int leftCount = 0;
    int rightCount = 0;

    if (drawFromBinaryDistribution())
    {
      leftCount = 1;
    }
    else
    {
      rightCount = 1;
    }

    intensity += (leftCount + rightCount);
  }

  std::cout << "Point intensity: " << intensity << std::endl;
  
  return 0;
}
