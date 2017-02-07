# Crowler

The project is developed in C, an URL same domain Crowler. Specifications (for now):
  - Only same domain (subdomains not included)
  - Only .html and .htm extension
  - Choose remove (or not) all files fetched
  - Only 1 level(s) deep 
  
In future, you'll can also:
  - Set deep level as you wish
  - Set extensions you want to get

### Libraries and Syscalls your compiler/systems MUST have:
* wget
* PThreads
* fork
* execve

On windows, you can install wget as "cmd.exe" command by install this [package][wget].

### Building and Running
Do NOT forget add this parameter at your build:
```sh
$ gcc -pthread...
```

### Output
The output can be 3 files (You can set this on/off, as change their names either, at settings.c):
  - links_valid.txt (All valid links [no repeat])
  - links_otherFiles.txt (All files with prohibited extensions)
  - links_otherDomains.txt (All files with other domains [including different subdomains])

### Author(s)
 - Paulo Mateus - [Twitter][PMattLink]

### Contributing

Want to contribute? Great! Let me know at this [link][contributing]!

## License
---
    Copyright (C) 2017 Paulo Mateus

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

         http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
**extreme coding :)**

   [PMattLink]: <http://twitter.com/paulomatew>
   [wget]: <http://gnuwin32.sourceforge.net/packages/wget.htm>
   [contributing]: <https://github.com/TroniPM/Crowler/issues/new>
