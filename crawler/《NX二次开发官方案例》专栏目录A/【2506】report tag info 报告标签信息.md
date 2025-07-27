### 【2506】report tag info 报告标签信息

#### 代码

```cpp
    /*HEAD REPORT_TAG_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void report_object_info(char *name, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            msg[133],  
            part_fspec[MAX_FSPEC_SIZE+1] = { "" },  
            obj_name[UF_OBJ_NAME_LEN + 1];  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            sprintf(msg, "%s = %d, type = %d, subtype = %d, ", name, object, type,  
                subtype);  
            ECHO(msg);  
            if (UF_OBJ_is_object_a_promotion(object))   
            {  
                sprintf(msg, "promotion, ");  
                ECHO(msg);  
            }  
            if (UF_ASSEM_is_occurrence(object))   
            {  
                sprintf(msg, "occurrence, ");  
                ECHO(msg);  
            }  
            if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
            {  
                UF_CALL(UF_PART_ask_part_name( owner,  part_fspec ));  
                sprintf(msg, "owned by %d, known as %s\n ", owner, part_fspec);  
                ECHO(msg);  
            }  
            if (!UF_OBJ_ask_name(object, obj_name))  
            {   
                sprintf(msg, "named %s, ", obj_name);  
                ECHO(msg);  
            }  
            status = UF_OBJ_ask_status(object);  
            sprintf(msg, "status %d\n", status);  
            ECHO(msg);  
        }  
        else   
        {     
            sprintf(msg, "%s = %d (NULL_TAG)\n", name, object);  
            ECHO(msg);  
        }  
    }  
    static int prompt_for_an_integer(char *prompt, char *item, int number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        int  
            da[1];  
        strcpy(&menu[0][0], item);  
        da[0] = number;  
        resp = uc1607(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            return da[0];  
        }  
        else return 0;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object = NULL_TAG;  
        while ((object = (tag_t)prompt_for_an_integer("Enter object EID", "tag",  
                (int) object)) != NULL_TAG)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, TRUE));  
            report_object_info("object", object);  
            uc1601("Object highlighted", TRUE);  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发的应用程序，主要用于查询和显示NX对象的信息。其主要功能包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个函数report_object_info，用于查询和显示NX对象的信息，包括类型、子类型、是否为提升、是否为组件实例、拥有者、名称和状态。
> 3. 定义了一个函数prompt_for_an_integer，用于提示用户输入一个整数，即对象ID。
> 4. 定义了主函数do_it，用于循环提示用户输入对象ID，并调用report_object_info函数查询和显示对象信息，同时高亮显示对象。
> 5. 定义了ufusr函数，作为程序的入口，初始化NX环境，调用do_it函数，然后终止NX环境。
> 6. 定义了ufusr_ask_unload函数，用于卸载应用程序。
>
> 总的来说，这段代码通过NX提供的API实现了查询和显示NX对象信息的功能，具有一定的实用价值。
>
