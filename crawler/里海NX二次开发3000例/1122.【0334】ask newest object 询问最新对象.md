### 【0334】ask newest object 询问最新对象

#### 代码

```cpp
    static tag_t ask_newest_object(tag_t part)  
    {  
        tag_t  
            last = NULL_TAG,  
            object = NULL_TAG;  
        while ((object = UF_OBJ_cycle_all(part, last)) != NULL_TAG)  
        {  
            last = object;  
        }  
        return last;  
    }

```

#### 代码解析

> 这段代码是NX二次开发中的一个函数，名为ask_newest_object。其作用是在NX中查询指定部件(part)中的最新对象。具体来说：
>
> 1. 定义了两个tag_t类型的变量last和object，分别用于保存上一个遍历到的对象和当前遍历到的对象。其中，tag_t是NX中用于标识对象的唯一标识符。
> 2. 通过调用UF_OBJ_cycle_all函数遍历指定部件中的所有对象。这个函数会返回一个tag_t类型的对象标识符，如果已经遍历完所有对象，则返回NULL_TAG。
> 3. 在while循环中，通过判断object是否为NULL_TAG来判断是否已经遍历完所有对象。如果没有遍历完，继续执行循环体。
> 4. 在循环体中，更新last为当前遍历到的对象object。
> 5. 当遍历完所有对象后，last将保存着最后一个遍历到的对象，即最新创建的对象。
> 6. 最后返回last，即最新创建的对象的标识符。
>
> 综上所述，这个函数实现了在NX中查询指定部件中的最新创建的对象的功能。
>
