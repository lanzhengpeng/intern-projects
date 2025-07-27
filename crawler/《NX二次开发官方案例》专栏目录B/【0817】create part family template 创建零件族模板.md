### 【0817】create part family template 创建零件族模板

#### 代码

```cpp
    /*HEAD CREATE_PART_FAMILY_TEMPLATE CCC UFUN */  
    #include <stdarg.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_fam.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_ugmgr.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_BUFSIZE];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), _TRUNCATE, format, args);  
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
    static int allocate_string_array(int n_elements, int n_chars, char ***where)  
    {  
        int  
            ii,  
            resp;  
        *where = UF_allocate_memory(n_elements * sizeof(char *), &resp);  
        for (ii = 0; ii < n_elements; ii++)  
        {  
            (*where)[ii] = UF_allocate_memory(n_chars * sizeof(char), &resp);  
            if (resp) return resp;  
        }  
        return 0;  
    }  
    static void do_it(void)  
    {  
        logical  
            is_template,  
            modifiable,  
            nxman;  
        int  
            ii,  
            index;  
        tag_t  
            attributes[2],  
            family,  
            part = UF_ASSEM_ask_work_part();  
        char  
            family_name[MAX_FSPEC_SIZE+1],  
            partname[MAX_FSPEC_SIZE+1],  
            part_number[UF_UGMGR_PARTNO_SIZE+1],  
            part_revision[UF_UGMGR_PARTREV_SIZE+1],  
            part_file_type[UF_UGMGR_FTYPE_SIZE+1],  
            part_file_name[UF_UGMGR_FNAME_SIZE+1],  
            **values;  
        UF_FAM_attribute_data_t  
            attribute_data;  
        UF_FAM_family_data_t  
            family_data;  
        UF_FAM_member_data_t  
            member_data;  
        UF_CALL(UF_PART_is_family_template(part, &is_template));  
        if (is_template)  
        {  
            uc1601("Part is already a part family template!", TRUE);  
            return;  
        }  
        UF_CALL(UF_PART_ask_part_name(part, partname));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        if (nxman)  
        {  
            UF_CALL(UF_UGMGR_decode_part_file_name(partname, part_number,  
                part_revision, part_file_type, part_file_name));  
            strcpy(family_name, part_number);  
        }  
        else  
            UF_CALL(uc4574(partname, 2, family_name));  
        UF_CALL(UF_is_ugmanager_active(&nxman));  
        attribute_data.subtype = UF_fam_attr_name_subtype;  
        attribute_data.base_object = NULL_TAG;  
        attribute_data.rules = NULL;  
        attribute_data.value = family_name;  
        attribute_data.name = "DB_PART_NO";  
        UF_CALL(UF_FAM_create_attribute(&attribute_data, &attributes[0]));  
        attribute_data.name = "OS_PART_NAME";  
        UF_CALL(UF_FAM_create_attribute(&attribute_data, &attributes[1]));  
        family_data.subtype = UF_fam_part_subtype;  
        family_data.attribute_count = 2;  
        family_data.member_count = 0;  
        family_data.attributes = attributes;  
        family_data.name = family_name;  
        UF_CALL(UF_FAM_create_family(&family_data, &family));  
        ECHO("Created %s\n", family_name);  
        member_data.value_count = 2;  
        allocate_string_array(2, MAX_FSPEC_SIZE+1, &values);  
        member_data.values = values;  
        for (ii = 0; ii < 2; ii++)  
        {  
            if (nxman)  
            {  
                UF_CALL(UF_UGMGR_assign_part_number(family_name, part_file_type,  
                    part_number, &modifiable));  
            }  
            else  
            {  
                sprintf(part_number, "%s_%d", family_name, ii+1);  
            }  
            strcpy(values[0], part_number);  
            strcpy(values[1], part_number);  
            UF_CALL(UF_FAM_add_member(family, &member_data, &index));  
            ECHO("Added family member at %d\n", index);  
        }  
        UF_CALL(UF_MODL_update());  
        UF_CALL(UF_FAM_check_family_status(family));  
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

> 根据代码，这是一个用于在NX中创建零件族模板的UFUN函数。代码的主要功能包括：
>
> 1. 检查当前零件是否已经是零件族模板，如果是则直接返回。
> 2. 获取当前零件的名称，并根据是否激活UGManager来解析出零件号和文件名。
> 3. 创建两个属性"DB_PART_NO"和"OS_PART_NAME"。
> 4. 使用这两个属性创建一个新的零件族。
> 5. 循环添加两个新的零件族成员，根据零件号生成不同的零件号。
> 6. 更新模型并检查零件族状态。
> 7. 使用UF_initialize和UF_terminate初始化和关闭NX。
> 8. 提供了ufusr和ufusr_ask_unload两个入口点供NX调用。
>
> 总的来说，这段代码实现了在NX中根据当前零件创建一个包含两个成员的零件族模板的功能。
>
