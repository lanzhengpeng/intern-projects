### 【1022】delete component array 删除组件数组

#### 代码

```cpp
    /*HEAD DELETE_COMPONENT_ARRAY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_assem.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static logical prompt_for_text(char *prompt, char *text)  
    {  
        int  
            n,  
            resp;  
        resp = uc1600(prompt, text, &n);  
        if (resp == 3 || resp == 5) return TRUE;  
        return FALSE;  
    }  
    static tag_t ask_named_component_array(char *name)  
    {  
        int  
            ii,  
            n,  
            type;  
        tag_t  
            *arrays,  
            *comps,  
            found = NULL_TAG;  
        UF_ASSEM_iset_array_data_t  
            iset_data;  
        UF_ASSEM_mc_array_data_t  
            mc_data;  
        UF_ASSEM_ask_arrays_in_part(UF_PART_ask_display_part(), &n, &arrays);  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_ASSEM_ask_type_of_array(arrays[ii], &type));  
            if (type == UF_ASSEM_ISET_ARRAY)  
            {  
                UF_CALL(UF_ASSEM_ask_iset_array_data(arrays[ii],&iset_data,&comps));  
                if (!strcmp(iset_data.array_name, name)) break;  
            }  
            else  
            {  
                UF_CALL(UF_ASSEM_ask_mc_array_data(arrays[ii], &mc_data, &comps));  
                if (!strcmp(mc_data.array_name, name)) break;  
            }  
            UF_free(comps);  
        }  
        if (ii < n) found = arrays[ii];  
        UF_free(arrays);  
        return found;  
    }  
    static void do_it(void)  
    {  
        int  
            type;  
        tag_t  
            *comps,  
            iset;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1] = "";  
        UF_ASSEM_iset_array_data_t  
            iset_data;  
        UF_ASSEM_mc_array_data_t  
            mc_data;  
        while (prompt_for_text("Enter name of component array to delete", name))  
        {  
            iset = ask_named_component_array(name);  
            UF_CALL(UF_ASSEM_ask_type_of_array(iset, &type));  
            if (type == UF_ASSEM_ISET_ARRAY)  
                UF_CALL(UF_ASSEM_ask_iset_array_data(iset,&iset_data,&comps));  
            else  
                UF_CALL(UF_ASSEM_ask_mc_array_data(iset, &mc_data, &comps));  
            UF_CALL(UF_ASSEM_delete_array(iset, TRUE));  
            UF_CALL(UF_ASSEM_remove_instance(comps[0]));  
            UF_free(comps);  
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

> 这段NX二次开发代码的主要功能是删除组件数组。具体来说，它包括以下关键点：
>
> 1. 包含必要的NX API头文件，如uf.h, uf_part.h等，用于调用NX提供的函数。
> 2. 定义了report_error函数，用于报告函数调用错误，并将错误信息打印到系统日志或列表窗口。
> 3. 定义了prompt_for_text函数，用于提示用户输入组件数组的名称，并获取用户输入。
> 4. 定义了ask_named_component_array函数，用于根据名称查询组件数组，并返回其tag。
> 5. 定义了do_it函数，该函数首先提示用户输入要删除的组件数组名称，然后根据名称查询组件数组，获取其tag，最后调用UF_ASSEM_delete_array函数删除组件数组，并调用UF_ASSEM_remove_instance函数删除数组中的第一个组件实例。
> 6. 定义了ufusr函数，这是NX二次开发的标准入口函数，其中初始化NX系统，调用do_it函数执行删除组件数组的操作，然后终止NX系统。
> 7. 定义了ufusr_ask_unload函数，返回立即卸载标志，表示二次开发应用程序在关闭时会立即卸载。
>
> 总的来说，这段代码实现了通过用户输入的组件数组名称，查询并删除该组件数组的二次开发功能。
>
