# gettext test code

This project is example for using gettext library.

## Build this project

When you run make at top source directory, You will get i18n-msg program at
bin direcotry.
``` sh
$> make all
```

## Studing usage for gettext utility.

This program print a message to console simply. If you run it with 
`-l ja_JP -c UTF-8` option, you will have japanese translated message. But
 if you run with either following options, you will not have japanese translated message. I wonder about gettext library not working with some options. 

* `-l ja`
* `-l ja -c UTF-8`
* `-l ja_JP`
* `-l en`

Here is the directory structure for i18n-msg program.
```
bin
|-- i18n
|   |-- en
|   |   `-- LC_MESSAGES
|   |       `-- messages.mo
|   |-- en_US
|   |   `-- LC_MESSAGES
|   |       `-- messages.mo
|   |-- ja
|   |   `-- LC_MESSAGES
|   |       `-- messages.mo
|   `-- ja_JP
|       `-- LC_MESSAGES
|           `-- messages.mo
`-- i18n-msg
```

Here is procedures call sequence about gettext.

``` c
/* locale is the command option you specified. */
setloale(LC_MESSAGES, locale);

/* domain_dir is "absolute path to i18n directory" */
bindtextdomain("messages", domain_dir);

/* codeset is the command option you specified. */
bind_textdomain_codeset("messages", codeset);
```


### You can not set locale.

When you run the program with some of above options, you can not set locale. 

``` sh
$> bin/i18n-msg -l ja
failed to bind text domain with ja
```

As you see above, locale ja exits. but setlocale was failed.

### You get unexpected string with some options.

When you run the program with following option, you get unexpected string. 

```sh
$> bin/i18n-msg -l ja_JP
gettext????????????
```

I had a mmessage file with UTF-8 and my evnvironment LC_CTYPE is 'en.UTF-8'.
You could understand by reading man 7 locale that LC_CTYPE is used to display
string.


