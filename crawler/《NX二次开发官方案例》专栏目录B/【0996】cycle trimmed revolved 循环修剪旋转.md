### 【0996】cycle trimmed revolved 循环修剪旋转

#### 代码

```cpp
    /*HEAD CYCLE_TRIMMED_REVOLVED CCC UFUN */  
    #include <uf_modl.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是在 V18 版本中新增的函数。 */  
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
    static tag_t ask_next_face(tag_t part, tag_t face)  
    {  
        int  
            subtype,  
            type;  
        while ((! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face)))  
            && (face != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
            if (subtype == UF_solid_face_subtype) return face;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            counter = 0,  
            type;  
        tag_t  
            face = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            sheet;  
        while( ((face = ask_next_face( part, face)) != NULL_TAG))  
        {  
            if(FTN(uf5411)( &face) == 0)  
            {  
                UF_CALL( UF_MODL_ask_face_type( face, &type));  
                if( type == 19)  
                {  
                    UF_CALL( UF_MODL_ask_face_body( face, &sheet));  
                    UF_CALL( UF_OBJ_set_layer( sheet, 2)); /* move to layer 2 里海译:将move to layer 2翻译为“移动到第二层”。 */  
                    counter++;  
                }  
            }  
        }  
        printf("\n\nNumber of trimmed/revolved faces: %d\n", counter-1);  
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

> 这段代码是用于NX的二次开发，其主要功能是遍历当前显示部件中的所有面，并找出类型为“修剪/旋转”的面，并将这些面的主体移动到第二层。
>
> 主要步骤如下：
>
> 1. 初始化NX系统。
> 2. 获取当前显示的部件。
> 3. 循环遍历部件中的所有面，通过ask_next_face函数获取下一个面。
> 4. 对于每个面，调用一个自定义的函数FTN(uf5411)来判断面是否为“修剪/旋转”类型。如果是，则获取面的主体。
> 5. 将面的主体移动到第二层。
> 6. 统计“修剪/旋转”面的数量并打印。
> 7. 终止NX系统。
>
> 此外，代码中还定义了report_error函数来打印错误信息，并使用了UF_print_syslog函数来打印日志，这是NX 18版本新增的函数。
>
> 总体来说，这段代码实现了遍历部件、判断面类型、移动主体和打印统计信息的功能，是一段典型的NX二次开发代码。
>
