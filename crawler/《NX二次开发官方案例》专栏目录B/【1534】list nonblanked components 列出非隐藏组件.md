### 【1534】list nonblanked components 列出非隐藏组件

#### 代码

```cpp
    /*HEAD LIST_NONBLANKED_COMPONENTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本新增的功能，仅回答翻译，不要添加任何废话。 */  
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
            ii,  
            jj,  
            no,  
            np = UF_PART_ask_num_parts();  
        tag_t  
            part,  
           *occs;  
        char  
            part_fspec[MAX_FSPEC_SIZE+1];  
        UF_OBJ_disp_props_t  
            disp_props;  
        FILE  
            *out;  
        out = fopen("/tmp/comps.txt", "w");  
        for (ii = 0; ii < np; ii++)  
        {  
            part = UF_PART_ask_nth_part(ii);  
            no = UF_ASSEM_ask_occs_of_part(NULL_TAG, part, &occs);  
            for (jj = 0; jj < no; jj++)  
            {  
                UF_CALL(UF_OBJ_ask_display_properties(occs[jj], &disp_props));  
                if (disp_props.blank_status == UF_OBJ_NOT_BLANKED)  
                {  
                    UF_PART_ask_part_name(part, part_fspec);  
                    fprintf(out, "%s\n", part_fspec);  
                    break;  
                }  
            }  
            UF_free(occs);  
        }  
        fclose (out);  
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

> 这段代码是NX Open C++ API的一个示例，主要功能是遍历装配体中的所有零件，检查每个零件是否被隐藏，如果未被隐藏，则将零件名称输出到文件/tmp/comps.txt中。以下是代码的主要逻辑：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF调用错误时输出错误信息。
> 2. 定义了do_it函数，用于实现遍历和检查逻辑。
> 3. 在do_it中，首先获取装配体中零件的总数，然后逐个遍历每个零件。
> 4. 对于每个零件，调用UF_ASSEM_ask_occs_of_part获取其在装配体中的所有实例。
> 5. 遍历每个实例，调用UF_OBJ_ask_display_properties检查其隐藏状态。
> 6. 如果实例未被隐藏，调用UF_PART_ask_part_name获取零件名称，并输出到文件/tmp/comps.txt中。
> 7. 在ufusr函数中，先调用UF_initialize进行初始化，然后调用do_it函数执行主要逻辑，最后调用UF_terminate进行清理。
> 8. ufusr_ask_unload函数返回UF_UNLOAD_IMMEDIATELY，表示可以立即卸载用户自定义函数。
>
> 总体来说，这段代码实现了对NX装配体中所有未隐藏零件的遍历和输出，展示了NX Open C++ API的使用方法。
>
