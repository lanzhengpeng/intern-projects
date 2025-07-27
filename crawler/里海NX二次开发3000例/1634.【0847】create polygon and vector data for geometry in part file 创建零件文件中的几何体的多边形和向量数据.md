### 【0847】create polygon and vector data for geometry in part file 创建零件文件中的几何体的多边形和向量数据

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITENZ(X) if (X != 0) ECHO("%s = %d\n", #X, X);  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1];  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (cols > 0)  
                if (ii == 0) ECHO("%s = ", name);  
                else ECHO("%s", blanks);  
            for (jj = 0; jj < cols; jj++)  
            {  
                ECHO("%f", array[kk++]);  
                if (jj < cols - 1) ECHO("%s", ", ");  
            }  
            if (cols > 0) ECHO("%s", "\n");  
        }  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void report_j3d_entities(UF_DISP_j3d_entity_t entity_list)  
    {  
        int  
            cntr = 0;  
        WRITE_D( entity_list.eid );  
        WRITE_D( entity_list.vectors);  
        WRITE_D( entity_list.has_transform);  
        if (entity_list.has_transform )  
        {  
            WRITE16F((double *)entity_list.transform);  
        }  
        WRITE_D( entity_list.num_strips );  
        for( cntr = 0; cntr < entity_list.num_strips; cntr++ )  
            ECHO("Points per strip - # %d: %d\n",  
                   cntr, entity_list.points_per_strip[cntr]);  
        WRITE_D( entity_list.total_points );  
        for( cntr = 0; cntr < entity_list.total_points; cntr++ )  
        {  
            int offset = cntr * 3;  
            ECHO("Point %d: %f, %f, %f\n", cntr,  
                entity_list.point_list[offset],  
                entity_list.point_list[offset + 1],  
                entity_list.point_list[offset + 2]);  
         }  
        for( cntr = 0; cntr < entity_list.total_points; cntr++ )  
        {  
            if( entity_list.vectors == 0 )  
            {  
                int offset = cntr * 3;  
                ECHO("Normal %d: %f, %f, %f\n", cntr,  
                        entity_list.normal_list[offset],  
                        entity_list.normal_list[offset + 1],  
                        entity_list.normal_list[offset + 2]);  
            }  
        }  
        WRITE_D( entity_list.sheet );  
        WRITE3F( entity_list.color );  
        WRITE_S( entity_list.name );  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            wireframe = 0,  
            num_entities = 0;  
        UF_DISP_j3d_entity_t  
            *entity_list = NULL;  
        UF_CALL( UF_DISP_j3d_geometry( wireframe, &num_entities, &entity_list ));  
        WRITE_D( num_entities );  
        for(inx = 0; inx < num_entities; inx++ )  
        {  
            ECHO("\n");  
            WRITE_D( inx );  
            report_j3d_entities(entity_list[inx]);  
        }  
        UF_CALL(UF_DISP_j3d_free_geometry(num_entities, entity_list ));  
    }  
    /* 里海 */  
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

> 这段代码是NX Open C++ API编写的NX二次开发示例代码，用于获取NX中的J3D几何体信息并打印输出。主要功能包括：
>
> 1. 定义了ECHO宏，用于打印信息到日志窗口。
> 2. 定义了UF_CALL宏，用于调用NX API函数，并报告错误信息。
> 3. 定义了WRITE_D、WRITENZ、WRITE3F、WRITE16F、WRITE_S等宏，用于打印输出不同类型的数据。
> 4. 定义了write_double_array_to_listing_window函数，用于打印输出double数组。
> 5. 定义了write_string_to_listing_window函数，用于打印输出字符串。
> 6. 定义了report_j3d_entities函数，用于打印输出J3D实体信息。
> 7. 定义了do_it函数，用于获取J3D几何体信息并打印输出。
> 8. 定义了ufusr函数，作为NX二次开发用户入口函数，调用do_it函数。
> 9. 定义了ufusr_ask_unload函数，用于卸载二次开发程序。
>
> 整个代码通过NX Open C++ API获取J3D几何体信息，并采用宏和函数封装的方式，打印输出不同类型的数据，实现了对NX几何体的二次开发。
>
