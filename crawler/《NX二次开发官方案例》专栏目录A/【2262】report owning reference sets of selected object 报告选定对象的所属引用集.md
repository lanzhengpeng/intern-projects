### 【2262】report owning reference sets of selected object 报告选定对象的所属引用集

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        logical  
            member;  
        int  
            ii,  
            n_ref_sets;  
        tag_t  
            object,  
            *ref_sets;  
        char  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        while ((object = select_an_object("Report owning Reference sets"))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_ASSEM_is_ref_set_member(object, &member));  
            if (!member)  
            {  
                ECHO("not a member of any reference sets\n");  
                continue;  
            }  
            UF_CALL(UF_ASSEM_ask_ref_sets(object, &n_ref_sets, &ref_sets));  
            ECHO("member of %d reference sets:\n", n_ref_sets);  
            for (ii = 0; ii < n_ref_sets; ii++)  
            {  
                UF_CALL(UF_OBJ_ask_name(ref_sets[ii], refset_name));  
                ECHO(" %d.  %s\n", ii+1, refset_name);  
            }  
            UF_free(ref_sets);  
        }  
    }  
    /* qq3123197280 */  
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

> 这段代码是一段基于NX的二次开发代码，主要功能是列出所选实体所属的引用集。
>
> 主要步骤包括：
>
> 1. 定义了ECHO函数，用于在列表窗口和日志中输出信息。
> 2. 定义了UF_CALL宏，用于调用NX API函数并报告错误。
> 3. 定义了report_error函数，用于报告API调用错误。
> 4. 定义了select_an_object函数，用于选择一个实体。
> 5. 定义了do_it函数，用于遍历每个实体，并查询它所属的引用集，然后输出引用集名称。
> 6. 定义了ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 通过以上步骤，代码实现了查询实体所属引用集的功能。
>
