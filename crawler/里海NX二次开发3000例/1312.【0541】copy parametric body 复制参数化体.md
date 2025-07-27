### 【0541】copy parametric body 复制参数化体

#### 代码

```cpp
    /*HEAD COPY_PARAMETRIC_BODY CCC UFUN */  
    /* THIS PROGRAM WILL CREATE THE ILLUSION OF A FULLY PARAMETRIC COPY OF A SOLID  
    BODY. IT WILL GIVE THE USER THE ABILITY TO SELECT BODIES TO MAKE A COPY OF AND  
    THE OPTION TO PLACE THE COPY ANYWHERE IN THE SCREEN. THEY WILL HAVE TO MOVE  
    THE COPIED BODY MANUALLY IF POSITION IS NOT DESIRED. THE ILLUSION IS DONE BY  
    EXPORTING THE DESIRED BODY INTO A PART FILE AND IMPORTING IT BACK TO ITSELF. THE  
    EXPRESSION ARE THEN MANIPULATED SO THAT THE EXPRESSIONS THAT HAVE "'" WILL BE  
    MODIFIED SO THAT THEIR VALUES WILL EQUAL THE ORIGINAL EXPRESSION NAME. EXAMPLE:  
    p0'0 = 2; WILL BE MODIFIED TO p0'0 = p0. AFTER THE REVISION HAS BEEN PERFORMED,  
    IT WILL IMPORT THE NEW EXPRESSION AND OVERWRITE THE EXISTING. HENCE , THE PARAMETRIC  
    COPY IS MADE. THIS CODE IS DESIGNED FOR INTERNAL EXECUTION ONLY.  */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #define MAX_LENGTH  132  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译如下：

注意：UF_print_syslog是V18版本新增的，请只提供翻译，不要添加其他内容。 */  
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
    static void manipulate_expression_data(void)  
    {  
        int  
            counter,  
            no_lines;  
        FILE  
            *exp_lines,  
            *write_file,  
            *exp_file;  
        char  
            *word_pointer = NULL,  
            exp_val[MAX_LENGTH],  
            exp_name[MAX_LENGTH],  
            temp_name[MAX_LENGTH];  
        system("wc -l /tmp/junk.exp > /tmp/exp_Count.dat"); /* read no of lines of text 译:读取文本行数 */  
        exp_lines = fopen("/tmp/exp_Count.dat", "r");  
        fscanf(exp_lines,"%d",&no_lines);  
        fclose(exp_lines);  
        exp_file = fopen("/tmp/junk.exp", "r");  
        write_file = fopen("/tmp/new_junk.exp", "w");  
        for(counter = 0; counter < no_lines; counter ++)  
        {  
            fscanf(exp_file,"%s\n", exp_name); /* get one line at a time 译:逐行获取 */  
            strcpy(temp_name, exp_name); /* create a copy of the name 译:创建名称的副本 */  
            if(strchr(exp_name,'\'')) /* check if expression has "'" 译:检查表达式是否包含单引号 */  
            {  
                word_pointer = strtok(exp_name,"="); /* parse name until "=" 译:翻译 parse name until "=" */  
                strcpy(exp_name, word_pointer);  
                word_pointer = strtok(temp_name,"'");/* parse name until "'" 译:翻译：解析名称直到"'" */  
                strcpy(exp_val, word_pointer);  
                strcpy(temp_name,exp_name); /* add it all up to create exp 译:把这一切都加起来创造经验。 */  
                strcat(temp_name,"=");  
                strcat(temp_name,exp_val);  
                strcat(temp_name,"\n");  
                fprintf(write_file, temp_name); /* write to new exp file 译:根据上下文，"write to new exp file"的意思是“写入新的经验文件”。 */  
            }  
            else  
            {  
                strcat(temp_name,"\n");  
                fprintf(write_file, temp_name);  
            }  
        }  
        fclose(exp_file);  
        fclose(write_file);  
        system("rm -f /tmp/exp_Count.dat"); /* cleaning up after myself 译:整理好自己的东西 */  
    }  
    static logical select_bodies(char *prompt, tag_p_t *bodies, int *cnt)  
    {  
        int  
            i,  
            resp;  
        UF_UI_selection_options_t  
            opts;  
        UF_UI_mask_t  
            mask;  
        opts.num_mask_triples = 1;  
        opts.mask_triples = &mask;  
        opts.scope = UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY;  
        mask.object_type = UF_solid_type;  
        mask.object_subtype = 0;  
        mask.solid_type = UF_UI_SEL_FEATURE_BODY;  
        UF_CALL(UF_UI_select_by_class(prompt, &opts, &resp, cnt, bodies));  
        if (resp == 3)  
        {  
            for (i=0; i<*cnt; i++)  
                UF_CALL(UF_DISP_set_highlight(*(*bodies+i),FALSE));  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static logical select_pos(char *prompt, double pos[3])  
    {  
        int  
            resp = uc1615(prompt, pos);  
        if (resp == 5) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            grp_id, /*group tag from file import */  
            wcs_mx, /*matrix tag */  
            wcs_ent, /* wcs tag 译:根据wcs tag的内容，我将其翻译为：

WCS标签翻译如下：

[1] WCS标签说明

[2] WCS标签格式

[3] WCS标签字段说明

[4] WCS标签示例

[5] WCS标签生成方法

[6] WCS标签打印要求

[7] WCS标签常见问题解答 */  
            obj_tag; /* dummy tag, just an address holder 译:翻译dummy tag, just an address holder为“仅作为地址占位符的虚拟标签”。 */  
        tag_p_t  
            body_tag = &obj_tag;  
        double  
            pos[3], /* position of file-> import 译:文件->导入的英文翻译为 "File -> Import"。 */  
            wcs[12]; /* wcs orientation  译:wcs orientation的翻译是wcs方向。 */  
        int  
            body_count; /* number of bodies selected  译:体数选择数 */  
        UF_import_part_modes_t  
            import_mode; /* import part settings  译:Import part settings 的翻译是：导入部件设置。 */  
        import_mode.layer_mode = 1;  
        import_mode.group_mode = 0;  
        import_mode.csys_mode = 0;  
        import_mode.plist_mode = 0;  
        import_mode.view_mode = 0;  
        import_mode.cam_mode = 0;  
        import_mode.use_search_dirs = 0;  
        select_bodies("Select bodies for parametric copy", &body_tag, &body_count);  
          /*  export selected bodies 译:导出选定实体 */  
        UF_CALL(UF_PART_export("/tmp/import",body_count,body_tag));  
         /* get the matrix of the WCS 译:获取WCS的矩阵 */  
        UF_CALL(UF_CSYS_ask_wcs(&wcs_ent));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs_ent, &wcs_mx, wcs));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, &wcs[3]));  
        select_pos("Select position to place body", pos);  
          /* import part that was just exported 译:Import part that was just exported */  
        UF_CALL(UF_PART_import("/tmp/import", &import_mode, &wcs[3], pos, 1.0, &grp_id));  
         /* get the current expression list 译:获取当前表达式列表 */  
        UF_CALL(UF_MODL_export_exp("/tmp/junk"));  
        manipulate_expression_data();  
         /* import the new and improved expression file 译:翻译：导入新的改进的表达文件 */  
        UF_CALL(UF_MODL_import_exp("/tmp/new_junk",0));  
         /* cleaning  up after myself 译:清理自己的残局 */  
        system("rm -f /tmp/import.prt");  
        system("rm -f /tmp/junk.exp");  
        system("rm -f /tmp/new_junk.exp");  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容，这是用于NX的二次开发代码，其主要功能是创建一个参数化复制的实体。代码的关键部分如下：
>
> 1. 导出实体：首先选择一个或多个实体，并将它们导出到一个prt文件中。
> 2. 导入实体：然后，选择一个位置来放置这个导出的实体，并将其重新导入到NX中。
> 3. 处理表达式：代码获取实体的表达式，并对其进行处理，将包含单引号的表达式中的值替换为表达式的名称，从而创建参数化复制的效果。
> 4. 导入处理后的表达式：最后，导入处理后的表达式，完成参数化复制的创建。
> 5. 清理文件：在完成操作后，代码会清理产生的临时文件。
> 6. 错误报告：代码中还包含一个错误报告函数，用于输出错误信息。
> 7. 主函数：主函数ufusr负责初始化和清理NX，并调用执行复制操作的函数。
> 8. 卸载函数：卸载函数ufusr_ask_unload返回立即卸载。
>
> 总的来说，这段代码实现了NX中参数化复制的功能，通过导出实体、处理表达式并重新导入来达到这一效果。
>
