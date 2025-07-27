### 【1519】list cset members 列出组件集的成员

#### 代码

```cpp
    /*HEAD LIST_CSET_MEMBERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog是V18版本新增的功能，请只回答翻译内容，不要添加任何废话。 */  
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
    static void do_it(void)  
    {  
        int  
            dev,  
            num_char,  
            ii;  
        tag_t  
            mem_tag[1000],  
            comp_tag = NULL_TAG,  
            cset_tag = NULL_TAG,  
            part_tag;  
        char  
            obj_name[30],  
            list_header[132],  
            cset_name[] = "COMP_SET";  
        logical  
            is_mem;  
        part_tag = UF_PART_ask_display_part();  
        uc1600("Please enter cset name to process", cset_name, &num_char);  
        (uc5203)(&cset_tag, cset_name, strlen(cset_name));  
        uc4400(0, "", 0);  
        FTN(uf4401)(&dev);  
        strcpy(list_header, "           Members of component set : ");  
        strcat(list_header, cset_name);  
        uc4403(list_header);  
        ii = 0;  
        while(!UF_CALL(UF_OBJ_cycle_objs_in_part(part_tag, UF_component_type,  
            &comp_tag)) && (comp_tag != NULL_TAG))  
        {  
            UF_CALL(UF_ASSEM_is_member_of_cset(cset_tag, comp_tag, &is_mem));  
            if(is_mem)  
            {  
                mem_tag[ii] = comp_tag;  
                uc5027(comp_tag, obj_name, &num_char);  
                uc4403(obj_name);  
                ii++;  
            }  
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

> 这段代码是NX CAD软件的二次开发示例代码，主要功能是列出组件集中的所有组件。代码主要包含以下部分：
>
> 1. 头文件包含：引入了UF函数库、对象类型、装配等相关的头文件。
> 2. 错误报告函数：定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 3. 主要功能函数：定义了do_it函数，该函数首先获取当前显示的部件，然后获取用户输入的组件集名称。接着遍历该部件的所有组件，判断是否属于输入的组件集，如果是则打印组件名称。
> 4. ufusr主函数：这是NX二次开发程序的入口函数。首先初始化UF库，然后调用do_it函数，最后终止UF库。
> 5. 卸载函数：定义了一个卸载函数，表示程序可以在任何时候被卸载。
>
> 总的来说，这段代码实现了根据用户输入的组件集名称，列出该组件集中所有组件的功能。通过UF函数库操作NX对象，实现了二次开发的目的。
>
