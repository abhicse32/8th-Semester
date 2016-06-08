; ModuleID = 'test5.c'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: nounwind uwtable
define void @foo(i32*** %p) #0 {
entry:
  %p.addr = alloca i32***, align 8
  store i32*** %p, i32**** %p.addr, align 8
  ret void
}

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32****, align 8
  %i = alloca i32***, align 8
  %j = alloca i32**, align 8
  %k = alloca i32*, align 8
  store i32 0, i32* %retval
  %call = call noalias i8* @malloc(i64 40) #2
  %0 = bitcast i8* %call to i32****
  store i32**** %0, i32***** %a, align 8
  %call1 = call noalias i8* @malloc(i64 40) #2
  %1 = bitcast i8* %call1 to i32***
  store i32*** %1, i32**** %i, align 8
  %call2 = call noalias i8* @malloc(i64 40) #2
  %2 = bitcast i8* %call2 to i32**
  store i32** %2, i32*** %j, align 8
  %call3 = call noalias i8* @malloc(i64 20) #2
  %3 = bitcast i8* %call3 to i32*
  store i32* %3, i32** %k, align 8
  %4 = load i32**** %i, align 8
  %5 = load i32***** %a, align 8
  store i32*** %4, i32**** %5, align 8
  %6 = load i32*** %j, align 8
  %7 = load i32**** %i, align 8
  store i32** %6, i32*** %7, align 8
  %8 = load i32** %k, align 8
  %9 = load i32*** %j, align 8
  store i32* %8, i32** %9, align 8
  %10 = load i32**** %i, align 8
  call void @foo(i32*** %10)
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
