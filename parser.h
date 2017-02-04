/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   parser.h
 * Author: matt
 *
 * Created on February 3, 2017, 10:55 PM
 */

#ifndef PARSER_H
#define PARSER_H

void openLinkFile();
void closeLinkFile();
void writeLinkOnFile(char *txt);
void tratarLinha(char * linha);
int parserInit(char * filename);
//#ifdef __cplusplus
//extern "C" {
//#endif
//#ifdef __cplusplus
//}
//#endif
#endif /* PARSER_H */

