/******************************************************************************
 * Copyright 2009-2012 Matteo Valdina
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "zCommon.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#include "zThread.h"
#include "zRect.h"

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "zGetOpt.h"
#include "zLogger.h"
#include "zFile.h"
#include "zStringBuilder.h"

#include "zWin.h"
#include "zWinFactory.h"

#include "GuiTest.h"

#if defined(ENABLED_DMALLOC)
#  include <dmalloc.h>
#endif

#if defined(WIN32) && defined(DEBUG)
# define _CRTDBG_MAP_ALLOC
# include <stdlib.h>
# include <crtdbg.h>
#endif


zLogger* g_logger = NULL;

void showCopyright(char const* programName);
void showHelp(char const* programName, zGetOpt* getopt);

#if defined(DMALLOC)
  void  dmalloc_track_function(const char *file, const unsigned int line, 
                              const int func_id,
                              const DMALLOC_SIZE byte_size,
                              const DMALLOC_SIZE alignment,
                              const DMALLOC_PNT old_addr,
                              const DMALLOC_PNT new_addr) {
    if (byte_size == 0) {
      // For some reason deletes are invoked with byte_size == 0.
      //printf("op %d file %s line %u size = 0\n", func_id, file, line);
    }
  }
#endif                                    


int main(int argc, char const** argv) {

  zGetOpt* getopt = new zGetOpt(argc, argv);
  getopt->add_arg('h', "help", false, "Show this message.");
  getopt->add_arg('v', "version", false, "Show version information and exit..");
  
  zGetOpt::Argument const* arg = NULL;
  bool interactive = false;
  bool use_dmalloc = false;

  while ((arg = getopt->next()) != NULL) {
    switch (arg->arg) {
    case 'h':
      showHelp(argv[0], getopt);
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      showCopyright(getopt->get_program_name().get_buffer());
      exit(EXIT_SUCCESS);
      break;
    case 'i':
      interactive = true;
      break;
    case 'd':
      use_dmalloc = true;
      break;
    }
  }
  showCopyright(getopt->get_program_name().get_buffer());

  if (getopt->get_error() != zGetOpt::ERR_NO_ERROR) {
    printf("%s\n", getopt->get_error_message().get_buffer());
    printf("Try '%s -h' for more information.\n", getopt->get_program_name().get_buffer());
    exit(EXIT_FAILURE);
  }

  g_logger = zLogger::get_logger("zanbase_test_runner");

  // Create all window.
  GuiTest* gui = new GuiTest();
  zWinFactory* factory = zWinFactory::get_instance();
  zWin* win = factory->create(zRect(10, 10, 210, 210), "Main window", gui);
  win->set_background_color(zSolidColor(0, 0, 0));
  // 
  factory->wait_all_windows_close();

  factory->shutdown();
  win->release_reference();
  g_logger->shutdown();
  g_logger->release_reference();
  delete getopt;
  delete gui;

#if defined(_CRTDBG_MAP_ALLOC)
  int found = _CrtDumpMemoryLeaks();
  if (found > 0) {
    DebugBreak();
  }
#endif
  return EXIT_SUCCESS;
}


void showCopyright(char const* programName) {
  bool debug = false;
#if defined(DEBUG)
  debug = true;
#endif
  printf("%s version %s %s\n", programName, PACKAGE_VERSION, (debug ? "Debug" : "Release"));
  printf("Copyright 2009-2012 Matteo Valdina (bugs: %s)\n", PACKAGE_BUGREPORT);
  printf("\n");
}


void showHelp(char const* programName, zGetOpt* opt) {
  //printf("Usage: %s [OPTION]...\n", programName);
  printf("%s\n", opt->get_usage_message().get_buffer());
  printf("This program executes a set of tests for the zanbase library.\n");
  printf("%s\n", opt->get_help_message().get_buffer());
  printf("Report %s bugs to %s.\n", programName, PACKAGE_BUGREPORT);
}
