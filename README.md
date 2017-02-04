# Crowler

The project is developed in C, an URL same domain Crowler. Specifications (for now):
  - Only same domain
  - Only .html and .htm extension
  - Only 5 levels deep

In future, you'll can also:
  - Set deep level as you wish
  - Set extensions you want to get
  - Choose remove (or not) all files fetched

### Libraries ans Syscalls
* wget
* PThreads
* fork
* execve

### Building and Running
Do NOT forget add this parameter at your build:
```sh
$ gcc -pthread...
```
The output file will be saved at same source folder as "links.txt".
### Developed By
 - Paulo Mateus - [Twitter][PMattLink]

### Development

Want to contribute? Great! Let me know!

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
**extreme coding, oh Yeah!**

   [PMattLink]: <http://twitter.com/paulomatew>
