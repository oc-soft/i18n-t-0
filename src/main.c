#include <stdio.h>
#include <libintl.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <locale.h>
#include <getopt.h>


/**
 * i18n option 
 */
typedef struct _i18n_option i18n_option;


/**
 * option
 */
struct _i18n_option {
    /**
     * locale
     */
    char* locale_str;

    /**
     * codeset
     */
    char* codeset;

    /**
     * request display help message
     */
    int help;
};


/**
 *  get message
 */
static char*
get_message();

/**
 *  free allocated message. 
 */
void
free_str(
    char* str);

/**
 *  get executable directory 
 */
static char*
get_executable_dir();

/**
 * get executable path
 */
static char*
get_executable_path();

/**
 * get executable name
 */
static char*
get_executable_name();


/**
 *  bind text domain and setlocale
 */
int
i18n_bindtext_domain(
    const char* locale_str,
    const char* codeset);


/**
 *  set locale 
 */
int
i18n_set_locale(
    const char* locale_str);

/**
 * print help message 
 */
void
i18n_print_help();

/**
 * create option block
 */
i18n_option*
i18n_option_create();

/**
 * create option block
 */
void
i18n_option_free(
    i18n_option* obj);

/**
 *  set locale
 */
int
i18n_option_set_locale(
    i18n_option* obj,
    const char* locale);

/**
 * get locale
 */
char*
i18n_option_get_locale_ref(
    i18n_option* obj);

/**
 *  set codeset 
 */
int
i18n_option_set_codeset(
    i18n_option* obj,
    const char* codeset);

/**
 * get codeset
 */
char*
i18n_option_get_codeset_ref(
    i18n_option* obj);


/**
 * parse option
 */
int
i18n_option_parse(
    i18n_option* obj,
    int argc,
    char* argv[]);


int
main(
    int argc,
    char* argv[]) {
    int result;
    char* msg;
    i18n_option* option_obj;
    result = 0;
    option_obj = NULL;
    option_obj = i18n_option_create();
    result = i18n_option_parse(option_obj, argc, argv);
    msg = NULL;
    if (option_obj && option_obj->help) {
        i18n_print_help();
    }
    if (i18n_option_get_locale_ref(option_obj)) {
        result = i18n_bindtext_domain(
            i18n_option_get_locale_ref(option_obj),
            i18n_option_get_codeset_ref(option_obj)); 
        if (result) {
            printf("failed to bind text domain with %s\n",
                i18n_option_get_locale_ref(option_obj)); 
        }
    }
    if (result == 0) { 
        msg = get_message();
    }
    if (msg) {
        printf("%s\n", msg);
    }
    if (msg) {
        free_str(msg);
    }
    if (option_obj) {
        i18n_option_free(option_obj);
    }
    return result;
}

/**
 * create option block
 */
i18n_option*
i18n_option_create()
{
    i18n_option* result;
    result = NULL;
    result = (i18n_option*)malloc(sizeof(*result));
    if (result) {
        result->locale_str = NULL;
        result->codeset = NULL;
        result->help = 0;
    }
    return result;
}

/**
 * create option block
 */
void
i18n_option_free(
    i18n_option* obj)
{
    if (obj) {
        i18n_option_set_locale(obj, NULL);
        i18n_option_set_codeset(obj, NULL);
        free(obj); 
    }
}

/**
 *  set locale
 */
int
i18n_option_set_locale(
    i18n_option* obj,
    const char* locale_str)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = 0;
        do_set = obj->locale_str != locale_str;
        if (do_set) {
            char* new_locale;
            new_locale = NULL;
            if (locale_str) {
                size_t length;
                length = strlen(locale_str);
                new_locale = (char*)malloc(length + 1); 
                if (new_locale) {
                    memcpy(new_locale, locale_str, length + 1);
                } else {
                    result = -1;
                }
            }
            if (result == 0) {
                if (obj->locale_str) {
                    free_str(obj->locale_str);
                }
                obj->locale_str = new_locale;
            }
        }
    }
    return result;
}

/**
 * get locale
 */
char*
i18n_option_get_locale_ref(
    i18n_option* obj)
{
    char* result;
    result = NULL;
    if (obj) {
        result = obj->locale_str;
    }
    return result;
}

/**
 *  set codeset
 */
int
i18n_option_set_codeset(
    i18n_option* obj,
    const char* codeset)
{
    int result;
    result = 0;
    if (obj) {
        int do_set;
        do_set = 0;
        do_set = obj->codeset != codeset;
        if (do_set) {
            char* new_codeset;
            new_codeset = NULL;
            if (codeset) {
                size_t length;
                length = strlen(codeset);
                new_codeset = (char*)malloc(length + 1); 
                if (new_codeset) {
                    memcpy(new_codeset, codeset, length + 1);
                } else {
                    result = -1;
                }
            }
            if (result == 0) {
                if (obj->codeset) {
                    free_str(obj->codeset);
                }
                obj->codeset = new_codeset;
            }
        }
    }
    return result;
}

/**
 * get codeset 
 */
char*
i18n_option_get_codeset_ref(
    i18n_option* obj)
{
    char* result;
    result = NULL;
    if (obj) {
        result = obj->codeset;
    }
    return result;
}



/**
 * parse option
 */
int
i18n_option_parse(
    i18n_option* obj,
    int argc,
    char* argv[])
{
    int result;
    static struct option options[] = {
        { "locale", required_argument, NULL, 'l' },
        { "codeset", required_argument, NULL, 'c' },
        { "help", no_argument, NULL, 'h' },
        { NULL, no_argument, NULL, 0 }
    }; 
    result = 0;
    do {
        int c;
        int opt_idx;
        opt_idx = 0;
        c = getopt_long(argc, argv, "hl:c:", options, &opt_idx);
        
        switch (c) {
        case 'h':
            if (obj) {
                obj->help = 1;
            }
            break;
        case 'l':
            result = i18n_option_set_locale(obj, optarg);
            break;
        case 'c':
            result = i18n_option_set_codeset(obj, optarg);
            break;
        case -1:
            break;
        default:
            if (obj) {
                obj->help = 1;
            }
            result = -1;
            c = -1;
            break;
        }
        if (c == -1) {
            break;
        }
        if (result) {
            break;
        }
    } while(1);
    return result;
}
/**
 * print help message 
 */
void
i18n_print_help()
{
    static struct {
        /**
         * long option
         */ 
        char* long_opt;
        /**
         * short option
         */
        char* short_opt;
        /**
         * descrition
         */
        char* description[1];
    } option_desc[] = {
        {
            .long_opt = "--locale=<LOCALE>",
            .short_opt = "-l",
            .description = { 
                "specify locale with which you want to see message." 
            }
        },
        {
            .long_opt = "--codeset=<CODESET>",
            .short_opt = "-c",
            .description = { 
                "specify codeset with which you want to see message." 
            }
        },
        {
            .long_opt = "--help",
            .short_opt = "-h",
            .description = { 
                "display this message" 
            }
        }
    }; 
    char* exe_name;
    size_t i;
    exe_name = get_executable_name();      
    if (exe_name) {
        printf("usage: %s [option]\n", exe_name);
    }
    for (i = 0; i < sizeof(option_desc) / sizeof(option_desc[0]); i++) {
        size_t j;
        const char* fmt = "%3s,% 20s  %s\n";
         
        printf(fmt, option_desc[i].short_opt,
            option_desc[i].long_opt, option_desc[i].description[0]);
        for (j = 1;
            j < sizeof(option_desc[i].description)
                / sizeof(option_desc[i].description[0]); j++) {
            printf(fmt, "", "", option_desc[i].description[j]);
        }
    }

    if (exe_name) {
        free_str(exe_name);
    }
}



/**
 *  bind text domain and setlocale
 */
int
i18n_bindtext_domain(
    const char* locale,
    const char* codeset)
{
    int result;
    result = i18n_set_locale(locale); 

    if (result == 0) {
        char* exe_dir;

        exe_dir = get_executable_dir(); 
        
        if (exe_dir) {
            char* domain_dir;
            const char* i18n_name = "i18n";
            size_t exe_dir_length;
            size_t domain_dir_length; 
            size_t i18n_length;
            exe_dir_length = strlen(exe_dir);
            i18n_length = strlen(i18n_name);
            domain_dir_length = exe_dir_length + i18n_length + 1;
            domain_dir = (char*)malloc(domain_dir_length);
            if (domain_dir) {
                char* cur_domain_base;
                snprintf(domain_dir, domain_dir_length, "%s%s",
                    exe_dir, i18n_name);
                cur_domain_base = bindtextdomain("messages", domain_dir); 
                result = cur_domain_base != NULL ? 0 : -1;
            }
            if (result == 0 && codeset) {
                char* cur_codeset;
                cur_codeset = bind_textdomain_codeset("messages", codeset);
                result = cur_codeset != NULL ? 0 : -1;
            }
            if (domain_dir) {
                free_str(domain_dir);
            }
        }
    }
    return result;
}


/**
 *  set locale 
 */
int
i18n_set_locale(
    const char* locale)
{
    char* locale_res;
    int result;
    locale_res = setlocale(LC_MESSAGES, locale); 
    if (!locale_res) {
        char* exec_dir;
        exec_dir = get_executable_dir();
        if (exec_dir) {
            size_t exec_dir_len;
            size_t locales_dir_len;
            size_t loc_path_buffer_size;
            const char* locales_dir = "locales";
            char* loc_path_buffer;
            char* saved_loc_path;
            locales_dir_len = strlen(locales_dir);
            exec_dir_len  = strlen(exec_dir);
            loc_path_buffer_size = exec_dir_len;
            loc_path_buffer_size += locales_dir_len;
            loc_path_buffer_size += 1;
            loc_path_buffer = (char*)malloc(loc_path_buffer_size);
            saved_loc_path = getenv("LOCPATH");
            if (loc_path_buffer) {
                snprintf(loc_path_buffer, loc_path_buffer_size,
                   "%s%s", exec_dir, locales_dir);    
                setenv("LOCPATH", loc_path_buffer, 1);
            }
            locale_res = setlocale(LC_MESSAGES, locale); 
            result = locale_res ? 0 : -1; 
            if (saved_loc_path) {
                setenv("LOC_PATH", saved_loc_path, 1);
            } 

            if (saved_loc_path) {
                free_str(saved_loc_path);
            }

            if (loc_path_buffer) {
                free_str(loc_path_buffer);
            }
        }
        if (exec_dir) {
            free_str(exec_dir);
        }    
    } else {
        result = 0;
    }
    return result;
}

/**
 *  get environment value.
 */
char*
i18n_get_env(
    const char* name)
{
    char* result;
    char* value;
    result = NULL;
    value = getenv(name);
    if (value) {
        size_t length;
        length = strlen(value);
        result = malloc(length + 1);
        if (result) {
            memcpy(result, value, length + 1);
        }
    }
    
    return result;
}


/**
 *  get message
 */
static char*
get_message()
{
    char* result;
    char* message;
    size_t len;
    message = gettext("I am using gettext library.");
    
    result = NULL;
    if (message) {
        size_t length;
        length = strlen(message);
        result = (char*)malloc(length + 1);
        if (result) {
            memcpy(result, message, length + 1);
        }
    }
    return result;
}

/**
 *  free allocated message. 
 */
void
free_str(
    char* str)
{
    if (str) {
        free(str);
    }
}

/**
 *  get executable directory 
 */
static char*
get_executable_dir()
{
    char* result;

    result = get_executable_path();
    if (result) {
        size_t dir_part_index;        
        size_t path_length;
        size_t i;
        dir_part_index = 0;
        path_length = strlen(result);
        for (i = 0; i < path_length; i++) {
            if (result[path_length - 1 - i] == '/') {
                dir_part_index = path_length - i;
                break;
            }
        }
        if (i < path_length) {
            result[dir_part_index] = '\0';
        } else {
            free_str(result);
            result = NULL;
        }
    }
    return result;
}
/**
 * get executable name
 */
static char*
get_executable_name()
{
    char* result;
    char* exe_full_path;
    
    exe_full_path = get_executable_path();
    result = NULL;
    if (exe_full_path) {
        size_t path_length;
        size_t i;
        path_length = strlen(exe_full_path);
        for (i = 0; i < path_length; i++) {
            if (exe_full_path[path_length - 1 - i] == '/') {
                break;
            }
        }
        if (i < path_length) {
            result = (char*)malloc(i + 1);
            if (result) {
                memcpy(result,
                    &exe_full_path[path_length - i], i + 1);
            }
        }
    }
    if (exe_full_path) {
        free_str(exe_full_path);
    }
    return result;

}


/**
 *  get executable path
 */
static char*
get_executable_path()
{
    char* result;
    size_t path_max;
#ifdef PATH_MAX
    path_max = PATH_MAX;
#else
    path_max = pathconf(path, _PC_PATH_MAX);
    if (path_max <= 0) {
        path_max = 4096;
    }
#endif 
    result = (char*)malloc(path_max);
    if (result) {
        ssize_t path_size;
        path_size = readlink("/proc/self/exe", result, path_max);
        if (path_size != -1) {
            result[path_size] = '\0';
        } else {
            free(result);
            result = NULL;
        }
    }
    return result;
}



/* vi: se ts=4 sw=4 et:*/
