# Crowler

The project is developed in C, an URL same domain Crowler. Specifications (for now):
  - Only same domain (subdomains not included);
  - Set the deep level;
  - Extensions to map are settable;
  - Choose write file with all links from OTHERS domains, all prohibited extensions and all emails found;

Extensions:
.html, .htm, .php, .rb, .rhtml, .dll, .cfm, .cgi, .svg, .py, jhtml, .xhtml, .swf, .asp, .aspx, .css, .js, .xml, .ico, .jpg, .jpeg, .png, .csp, .do, .jsf, .jspx, .pdf, .gif, .ps, .txt, .shar, .roff, .tgz, .zip, .rar, .tar, .csv, .exe, .bat, .rtf, .doc, .docx, .odt, .gz
  
## Parameters input (*= optional)

> - **link** = choose one link to make crowler work;
> - **level*** = set the deep level of crowler (default: 5);
> - **ext*** = set the allowed files extensions that crowler can access (default: **html** and **htm**). Use common (,) to separate extensions. By setting this, every the **others** extensions (listed above) won't be mapped;
> - **noerase*** = if you set this, all files (pages) catched will be available to you inside a folder named 'workspace_crowler';
> - **explicit*** = if you set this, the crowler only will map pages with explicit extensions. Example: 'www.openbsd.org/faq' won't be mapped. 'www.openbsd.org/faq/index.html' will be mapped;
> - **email*** = if you set this, the program will save every email found on file named 'links_emails.txt';
> - **otherdomains*** = if you set this, the program will save every link of other domain on a file named 'links_otherDomains.txt';
> - **otherfiles*** = if you set this, the program will save every file with prohibited extension on a file named 'links_otherDomains.txt';

  - Parameter **link** is required;
  - If you run with no parameters (or --help), you'll get some info about parameters and running process.

### Libraries and Syscalls your compiler/systems MUST have:
* wget
* fork
* execl

On windows, you can install wget as `cmd.exe` command by install this [package][wget].

### Building and Running
Example 1 : 
```sh
./crowler -link=subdomain.maindomain.com --noerase -level=5 -ext=html,htm,php --explicit --email --otherdomains --otherfiles
```
Example 2 : 
```sh
./crowler -link=subdomain.maindomain.com 
```
Example 3 : 
```sh
./crowler -link=subdomain.maindomain.com --explicit --email
```

### Output
 - The output can be 3 files, valid links not included here. You can set them ON by adding input parameters (--email, --otherdomains, --otherfiles) at start.
 - A folder  will be created to help the program process. By default, that folder is eliminated, but you can set this OFF adding the parameter "--noerase";

# !!BE CAREFUL `<DANGER>`!!
 - The entire process may lead to a memory overflow (depending of website arquiteture), given the amount of child processes generated. `[TO IMPROVE]`;
 - The entire process may take long time. We advise you to drink some coffee and play a bit at your smartphone.

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
