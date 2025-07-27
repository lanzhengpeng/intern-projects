### 【1483】is scenario part 当零件是场景部分时，返回0 (FALSE)

#### 代码

```cpp
    /* UF_SF_is_scenario_part returns 0 (FALSE) when the part IS a scenario part 译:当零件是场景部分时，UF_SF_is_scenario_part 返回 0 (FALSE)。 */  
    static logical is_scenario_part(void)  
    {  
        if (!UF_SF_is_scenario_part()) return TRUE;  
        else return FALSE;  
    }

```

```cpp
if (UF_SF_is_scenario_part() == 0) 
  return TRUE; 
else 
  return FALSE;

```

```cpp
if (!UF_SF_is_scenario_part()) 
  return FALSE; 
else 
  return TRUE;

```

#### 代码解析

> 这段代码是用于NX Open C++的二次开发，主要功能是判断当前打开的NX部件是否为场景的一部分。
>
> 代码逻辑如下：
>
> 1. 调用UF_SF_is_scenario_part()函数，该函数用于判断当前打开的NX部件是否为场景的一部分。
> 2. 如果UF_SF_is_scenario_part()返回值为0，表示当前部件是场景的一部分，因此返回TRUE。
> 3. 如果UF_SF_is_scenario_part()返回值不为0，表示当前部件不是场景的一部分，因此返回FALSE。
>
> 通过调用UF_SF_is_scenario_part()函数，可以判断当前打开的NX部件是否属于场景，这对于开发需要针对场景和普通部件进行不同处理的程序很有用。
>
> 需要注意的是，根据NX官方文档，UF_SF_is_scenario_part()函数返回0表示部件是场景的一部分，返回非0表示不是场景的一部分。所以代码中的逻辑判断应该是：
>
> ```cpp
> if (UF_SF_is_scenario_part() == 0) 
>   return TRUE; 
> else 
>   return FALSE;
> ```
>
> 因此，原代码中的逻辑判断是错误的，应该是：
>
> ```cpp
> if (!UF_SF_is_scenario_part()) 
>   return FALSE; 
> else 
>   return TRUE;
> ```
>
> 正确的代码应该如上所示，以正确判断当前部件是否为场景的一部分。
>
