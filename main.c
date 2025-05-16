/**
    StenoByte: a stenotype inspired keyboard app for typing out bytes.

	Copyright 2025 Asami De Almeida

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 */

#include "StenoByte_Core.h"


int main() {
    // Performs setup; exits app if there was an error while setting up
    const int setup_result = setup_stenobyte();
    if (setup_result != 0) {
        return setup_result;
    }

    // Runs the loop
    run_stenobyte();

    // Frees up Memory Safely
    end_stenobyte();
    return 0;
}
