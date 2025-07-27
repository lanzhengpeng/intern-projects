### 【1174】ensure fully loaded 确保完全加载

#### 代码

```cpp
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_CALL(UF_PART_free_load_status(status));  
        }  
    }  
    static void ensure_fully_loaded(tag_t item)  
    {  
        tag_t  
            part;  
        char  
            partname[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_load_status_t  
            error_status;  
        if (UF_ASSEM_is_occurrence(item))  
            item = UF_ASSEM_ask_prototype_of_occ(item);  
        UF_CALL(UF_OBJ_ask_owning_part(item, &part));  
        UF_PART_ask_part_name(part, partname);  
        if ((UF_PART_is_loaded(partname)) != 1)  
        {  
            UF_CALL(UF_PART_open_quiet(partname, &part, &error_status));  
            if (error_status.n_parts > 0) report_load_status(&error_status);  
        }  
    }

```

#### 代码解析

> 这段代码是用于NX CAD软件的二次开发，主要功能是确保组件完全加载。下面是对代码的详细解释：
>
> 1. report_load_status函数：用于报告组件加载状态。它接收一个UF_PART_load_status_p_t类型的参数，该参数包含加载状态信息。函数首先判断加载的组件数量，如果大于0，则遍历每个组件，获取其加载失败的错误消息，并打印出来。最后，释放加载状态信息内存。
> 2. ensure_fully_loaded函数：确保传入的组件或组件实例完全加载。首先判断传入的是组件实例还是组件原型，如果是实例，则获取其原型。然后获取组件的拥有者组件，并判断该组件是否已加载。如果未加载，则尝试静默加载，并获取加载状态。如果加载状态包含错误信息，则调用report_load_status函数报告错误。
> 3. 代码逻辑：首先定义了两个辅助函数，一个用于报告加载状态，一个用于确保组件完全加载。主逻辑是调用ensure_fully_loaded函数，传入要确保加载的组件或组件实例。该函数会递归地确保传入组件及其所有子组件都完全加载。
> 4. 适用场景：该代码适用于在NX CAD软件中，需要在操作前确保某些组件完全加载的场景，如进行模型装配、仿真分析等。通过调用ensure_fully_loaded函数，可以避免由于组件未加载而导致的错误。
> 5. 注意事项：在使用该代码时，需要注意传入的组件或组件实例的有效性，以及加载状态信息的正确处理，避免内存泄露等问题。
>
> 以上是对这段代码的详细介绍，如有任何其他问题，欢迎继续提问。
>
