### 【1818】read csv text file and create points in wcs 读取CSV文本文件，在WCS中创建点

#### 代码

```cpp
    /*HEAD READ_CSV_TEXT_FILE_AND_CREATE_POINTS_IN_WCS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_curve.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    static logical prompt_for_csv_text_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1];  
        strcpy(filter, "*.csv");  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "CSV Text File Name", filter, "",   
            fspec, &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            cntr = 1,  
            n_lines = 0;  
        tag_t  
            point = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            point_location[3] = { 0., 0., 0. },  
            abs_point_location[3] = { 0., 0., 0. };  
        char  
            msg[300] = { "" },  
            *txtptr = NULL,  
            pname[30] = { "" },  
            a_line[MAX_LINE_SIZE+1] = { "" },  
            fname[MAX_FSPEC_SIZE + 1] = { "" };  
        FILE   
            *csv_file = NULL;  
        if( NULL_TAG == part )  
        {  
            sprintf( msg, "No Current Part.  Program needs a part.\n\n" );  
            WRITE( msg );  
            return;     
        }  
        if(prompt_for_csv_text_name("Select CSV Text File:", fname))  
        {  
            csv_file = fopen( fname, "r" );  
            if( csv_file == NULL )  
            {  
                sprintf( msg, "Could not open the file, %s\n", fname);  
                WRITE( msg );  
                return;  
            }  
            else  
            {  
                while ((fgets(a_line, MAX_LINE_SIZE, csv_file)) != NULL) n_lines++;  
                rewind(csv_file);  
                for (inx = 0; inx < n_lines; inx++)  
                {  
                    fgets(a_line, MAX_LINE_SIZE, csv_file);  
                    if( (txtptr = strchr(a_line, ',')) == NULL ) continue;  
                    point_location[0] = atof( a_line );  
                    txtptr += sizeof( char );  
                    point_location[1] = atof( txtptr );  
                    txtptr = strchr(txtptr, ',' ) + sizeof( char );  
                    point_location[2] = atof( txtptr );  
                    /* translate points to ABS CSYS 里海译:将点转换到ABS坐标系。 */  
                    UF_CALL( UF_CSYS_map_point( UF_CSYS_ROOT_WCS_COORDS,  
                        point_location, UF_CSYS_ROOT_COORDS, abs_point_location));  
                    UF_CALL( UF_CURVE_create_point( abs_point_location, &point ));  
                    sprintf( pname, "POINT_%d", cntr++ );  
                    /* comment next line to avoid naming the points. 里海译:翻译：避免在下一行命名点。 */  
                    UF_CALL( UF_OBJ_set_name( point, pname ));  
                }  
                fclose( csv_file );  
                UF_VIEW_fit_view( NULL_TAG, 0.9 );  
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

> 这段代码是用于NX的二次开发，主要功能是读取CSV文件中的坐标点，并在当前部件中创建相应的点。以下是该代码的主要内容和功能：
>
> 1. 引入相关头文件：引入了NX二次开发所需的头文件，包括系统、对象、模型、部件、视图、曲线等模块的头文件。
> 2. 错误处理宏：定义了错误处理宏UF_CALL，用于调用NX API时捕获错误并输出错误信息。
> 3. 提示选择CSV文件：使用UF_UI_create_filebox函数弹窗提示用户选择CSV文件，并返回文件名。
> 4. 读取CSV文件：使用fopen、fgets等C标准库函数打开和读取CSV文件，统计行数。
> 5. 遍历CSV文件：逐行读取坐标点信息，并使用atof转换为浮点数。
> 6. 坐标转换：调用UF_CSYS_map_point函数将相对坐标转换为绝对坐标。
> 7. 创建点：使用UF_CURVE_create_point函数在当前部件中创建点。
> 8. 设置点名称：使用UF_OBJ_set_name函数为创建的点设置名称。
> 9. 视图适应：调用UF_VIEW_fit_view函数使视图适应显示所有创建的点。
> 10. UF_terminate结束：在函数末尾调用UF_terminate结束NX API的使用。
>
> 总体来说，这段代码实现了从CSV文件读取坐标点，并在NX部件中创建点的功能。它通过弹窗提示用户选择文件，读取文件内容，转换坐标，创建点，并设置点名称，最后调整视图。
>
