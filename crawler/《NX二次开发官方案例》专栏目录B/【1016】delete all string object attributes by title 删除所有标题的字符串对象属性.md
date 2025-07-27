### 【1016】delete all string object attributes by title 删除所有标题的字符串对象属性

#### 代码

```cpp
    /*HEAD DELETE_ALL_STRING_OBJECT_ATTRIBUTES_BY_TITLE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_layer.h>  
    #include <uf_assem.h>  
    #include <uf_undo.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_attr.h>  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是V18版本新增的。 */  
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
    static void report_and_delete_attributes(tag_t object, char *titlesub)  
    {  
        int  
            ii = 0,  
            indx = 0,  
            n;  
        char  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            titles[50][UF_ATTR_MAX_TITLE_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_string, &n));  
        if (n > 0)  
        {  
            sprintf(mstr, "\ntag %d has %d string attributes\n", object, n);  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && (indx != 0) && ii < 50)  
        {  
            if( att.type == UF_ATTR_string)  
            {  
                sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                UF_free(att.value.string);  
                UF_CALL(UF_UI_write_listing_window(mstr));  
                if(strstr(title, titlesub))   
                {  
                    strcpy(titles[ii], title);  
                    printf("titles[%d]: %s\n", ii, titles[ii]);  
                    printf("%d = ii\n",  ii++);  
                }  
            }  
        }  
        n = ii;  
        if(n > 0)  
        {  
            for (ii = 0; ii < n ; ii++)   
            {  
                printf("Trying to delete att w/title: %s\n", titles[ii]);  
                UF_CALL(UF_ATTR_delete(object, UF_ATTR_string, titles[ii]));  
            }  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object = NULL_TAG;  
        char  
            *titlesub = {"CONNECTED PART"};  
        prompt_for_text("Enter Attribute Title Substring", titlesub);  
        while (!UF_CALL(UF_LAYER_cycle_by_layer(0, &object)) &&  
            (object != NULL_TAG))  
        {  
            report_and_delete_attributes(object, titlesub);  
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
    static void set_all_layers_active(void)  
    {  
        int ii;  
        for (ii = 1; ii < 257; ii++)  
            UF_CALL(UF_LAYER_set_status(ii, UF_LAYER_ACTIVE_LAYER));  
    }  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    set_all_layers_active();  
                    do_it();  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的NX二次开发示例代码，主要用于删除具有特定标题子串的所有字符串属性。以下是该代码的主要功能：
>
> 1. 初始化NX Open环境。
> 2. 循环遍历所有图层，并对每个图层对象调用report_and_delete_attributes函数，该函数会循环遍历对象的所有字符串属性，并打印出具有特定标题子串的属性，然后删除这些属性。
> 3. 提示用户输入要删除的属性标题子串。
> 4. 如果代码以命令行参数启动，则会打开每个参数指定的零件，并设置所有图层为激活状态，然后删除所有指定标题子串的字符串属性。
> 5. 打印处理了多少个零件。
> 6. 终止NX Open环境。
>
> 该代码展示了NX Open C++ API的基本使用，包括错误处理、属性操作、图层操作、零件操作等。通过命令行参数支持，提高了代码的可用性。
>
