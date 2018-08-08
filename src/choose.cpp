// Copyright (C) 2018 Thomas Strunz
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
#include <cstring>
#include "token.h"
#include "screen.h"


int Screen::LeftScreenIntensity  = 0;
int Screen::RightScreenIntensity = 0;

#ifdef _MSC_VER
# include <intrin.h>
#endif


/* These are bits that are OR’d together */

#define DRNG_NO_SUPPORT	0x0	/* For clarity */
#define DRNG_HAS_RDRAND	0x1
#define DRNG_HAS_RDSEED	0x2


typedef struct cpuid_struct {
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;
} cpuid_t;


void cpuid (cpuid_t *info, unsigned int leaf, unsigned int subleaf)
{
#ifdef __GNUC__
  asm volatile("cpuid"
      : "=a" (info->eax), "=b" (info->ebx), "=c" (info->ecx), "=d" (info->edx)
      : "a" (leaf), "c" (subleaf)
      );
#elif _MSC_VER
  __cpuidex(reinterpret_cast<int*>(info), leaf, subleaf);
#endif
}


int _is_intel_cpu ()
{
  using std::memcmp;

  static int intel_cpu= -1;
  cpuid_t info;

  if ( intel_cpu == -1 ) {
    cpuid(&info, 0, 0);

    if (
        memcmp((char *) &info.ebx, "Genu", 4) ||
        memcmp((char *) &info.edx, "ineI", 4) ||
        memcmp((char *) &info.ecx, "ntel", 4)
       ) {
      intel_cpu= 0;
      //std::cout << "This is NOT an Intel CPU" << std::endl;
    } else {
      intel_cpu= 1;
      //std::cout << "This is an Intel CPU" << std::endl;
    }
  }

  return intel_cpu;
}


int get_drng_support ()
{
  static int drng_features= -1;

  /* So we don't call cpuid multiple times for
   * the same information */

  if ( drng_features == -1 ) {
    drng_features= DRNG_NO_SUPPORT;

    if ( _is_intel_cpu() ) {
      cpuid_t info;

      cpuid(&info, 1, 0);

      if ( (info.ecx & 0x40000000) == 0x40000000 ) {
        drng_features|= DRNG_HAS_RDRAND;
      }

      cpuid(&info, 7, 0);

      if ( (info.ebx & 0x40000) == 0x40000 ) {
        drng_features|= DRNG_HAS_RDSEED;
      }
    }
  }

  return drng_features;
}


int rdrand16_step (uint16_t *rand)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdrand %0; setc %1"
      : "=r" (*rand), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}

int rdrand32_step (uint32_t *rand)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdrand %0; setc %1"
      : "=r" (*rand), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}

int rdrand64_step (uint64_t *rand)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdrand %0; setc %1"
      : "=r" (*rand), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}

int rdrand16_retry (unsigned int retries, uint16_t *rand)
{
  unsigned int count= 0;

  while ( count <= retries ) {
    if ( rdrand16_step(rand) ) {
      return 1;
    }

    ++count;
  }

  return 0;
}

int rdrand32_retry (unsigned int retries, uint32_t *rand)
{
  unsigned int count= 0;

  while ( count <= retries ) {
    if ( rdrand32_step(rand) ) {
      return 1;
    }

    ++count;
  }

  return 0;
}

int rdrand64_retry (unsigned int retries, uint64_t *rand)
{
  unsigned int count= 0;

  while ( count <= retries ) {
    if ( rdrand64_step(rand) ) {
      return 1;
    }

    ++count;
  }

  return 0;
}


int rdseed16_step (uint16_t *seed)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdseed %0; setc %1"
      : "=r" (*seed), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}

int rdseed32_step (uint32_t *seed)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdseed %0; setc %1"
      : "=r" (*seed), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}

int rdseed64_step (uint64_t *seed)
{
  unsigned char ok {0};

#ifdef __GNUC__
  asm volatile ("rdseed %0; setc %1"
      : "=r" (*seed), "=qm" (ok));
#elif _MSC_VER

#endif

  return (int) ok;
}


//bool drawFromBinaryDistribution()
//{
//  auto ts = std::chrono::system_clock::now();
////  std::cout << "TS: " << ts.time_since_epoch().count() << std::endl;
//  std::cout << "RAN: " << (ts.time_since_epoch().count() %2 > 0) <<  std::endl;
//
//  return ts.time_since_epoch().count() %2 > 0;
//}

//bool drawFromBinaryDistribution()
//{
//  using std::cout;
//  using std::endl;
//
//  static int hwSupport = -1;
//
//  if (hwSupport == -1)
//  {
//    hwSupport = get_drng_support();
//    cout << "RDRAND is available: " << (((hwSupport & DRNG_HAS_RDRAND) == DRNG_HAS_RDRAND) ? "Yes" : "No") << endl;
//    cout << "RDSEED is available: " << (((hwSupport & DRNG_HAS_RDSEED) == DRNG_HAS_RDSEED) ? "Yes" : "No") << endl;
//    if (hwSupport == DRNG_NO_SUPPORT)
//    {
//      return false;
//    }
//  }
//
//  uint64_t rand = 0;
//
//  if (0 == rdrand64_retry(10, &rand))
//  {
//    cout << "There is no random number available!" << endl;
//    return false;
//  }
//
////  cout << "RAND " << rand << endl;
//  return (1ull << 61) > rand;
//}

bool drawFromBinaryDistribution()
{
  using std::cout;
  using std::endl;

  static int hwSupport = -1;

  if (hwSupport == -1)
  {
    hwSupport = get_drng_support();
    cout << "RDRAND is available: " << (((hwSupport & DRNG_HAS_RDRAND) == DRNG_HAS_RDRAND) ? "Yes" : "No") << endl;
    cout << "RDSEED is available: " << (((hwSupport & DRNG_HAS_RDSEED) == DRNG_HAS_RDSEED) ? "Yes" : "No") << endl;
    if (hwSupport == DRNG_NO_SUPPORT)
    {
      return false;
    }
  }

  uint16_t rand = 0;

  for (; 0==rdseed16_step(&rand) ;)
  {
  }

  return (1ull << 15) > rand;
}


Token* createToken()
{
  if (drawFromBinaryDistribution())
  {
    return new LeftToken;
  }
  else
  {
    return new RightToken;
  }
}


//Screen* getScreen()
//{
//  if (drawFromBinaryDistribution())
//  {
//    return new LeftScreen;
//  }
//  else
//  {
//    return new RightScreen;
//  }
//}


int main(int /*argc*/, char **)
{
  std::cout << "choose - random path demo                              Thomas Strunz (c) 2018 " << std::endl;

  for (int i=0; i<1000000; ++i)
  {
    Token* token = createToken();
    Screen* screen = token->getScreen();

    screen->hit();

    delete screen;
    delete token;
  }

  std::cout << "Point intensity: " << Screen::LeftScreenIntensity << std::endl;
  std::cout << "Point intensity: " << Screen::RightScreenIntensity << std::endl;

  return 0;
}
