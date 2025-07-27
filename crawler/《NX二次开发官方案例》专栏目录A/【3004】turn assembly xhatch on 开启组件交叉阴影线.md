### 【3004】turn assembly xhatch on 开启组件交叉阴影线

#### 代码

```cpp
    /*HEAD TURN_ASSEMBLY_XHATCH_ON CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_draw.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #define ECHOS(X)   (printf("%s = %s\n", #X, X))  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
        logical  
            is_a_sxview;  
        tag_t  
            d_tag = NULL_TAG,  
            p_tag = UF_PART_ask_display_part(),  
            v_tag;  
        char  
            view[MAX_ENTITY_NAME_SIZE+1],  
            drawing[MAX_ENTITY_NAME_SIZE+1];  
        UF_DRAW_sxview_prfs_t  
            sxview_parms;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(p_tag, UF_drawing_type, &d_tag))  
            && (d_tag != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_name(d_tag, drawing));  
            ECHOS(drawing);  
            strcpy(view, "");  
            while (!UF_CALL(uc6499(drawing, view)) && (strcmp(view, "")))  
            {  
                UF_CALL(UF_VIEW_ask_tag_of_view_name(view, &v_tag));  
                UF_CALL(UF_DRAW_is_sxview(v_tag, &is_a_sxview));  
                if (is_a_sxview)  
                {  
                    ECHOS(view);  
                    UF_CALL(UF_DRAW_ask_sxview_display(v_tag, &sxview_parms));  
                    sxview_parms.sx_assy_xhatch = UF_DRAW_sx_assy_xhatch_on;  
                    ECHO(sxview_parms.sx_assy_xhatch);  
                    UF_CALL(UF_DRAW_set_sxview_display(v_tag, &sxview_parms));  
                }  
            }  
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
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
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
            copy_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    build_copy_partname(copy_fspec);  
                    UF_CALL(UF_PART_save_as(copy_fspec));  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这是段NX二次开发代码，其主要功能是遍历当前打开的NX部件中的所有绘图，并针对每个绘图找到对应的视图，检查是否是X轴绘图，如果是，则打开X轴绘图的装配交叉线显示。
>
> 代码的主要流程包括：
>
> 1. 定义错误处理函数report_error，用于在函数调用出错时打印错误信息。
> 2. 定义主函数do_it，该函数遍历部件中的所有绘图，对于每个绘图，调用辅助函数uc6499获取对应的视图名，然后根据视图名获取视图标签，并判断是否是X轴绘图。如果是，则获取和设置X轴绘图的显示参数，打开装配交叉线显示。
> 3. 定义UF函数入口ufusr，调用do_it函数。
> 4. 定义主函数main，用于处理命令行参数，打开指定的部件文件，调用do_it函数，并保存修改后的部件为新的文件。
> 5. 定义辅助函数build_copy_partname，用于构建新文件的文件名。
> 6. 定义错误处理函数check_load_status，用于打印打开部件时的错误信息。
>
> 通过这段代码，可以方便地批量打开NX部件中X轴绘图的装配交叉线显示，提高了NX绘图的效率。
>
