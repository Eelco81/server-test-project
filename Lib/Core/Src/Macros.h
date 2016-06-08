
#ifndef _MACROS_H_
#define _MACROS_H_

#define NO_COPY_CONSTRUCTORS(classname) public:classname(const classname&)=delete;classname(classname&&)=delete;classname&operator=(classname const&)=delete;classname&operator=(classname &&)=delete;

#endif

