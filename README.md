The Data Oriented Orthodox Template Library is an attempt to remove and wrap the insanity of modern c++, replacing it with data oriented templates and data transform functions.  
All of the STL is strictly removed from headers, this removes approximately [40-400]kLOC from most compilation units, dramatically reducing compilation times, nearer what would be expected of C.  

DOOT is potentially multipurpose, but is intended for gamedev


Warps  
 are used for multithreading. A group of threads that behave as the data-oriented pattern of tasker.  
Invoked by taking an array of jobs, and dividing the jobs evenly among the threads of the warp.  
Warps maynt access the scope of other warps.  
 Warp-scope may be read immutably while warping.  
 Mutations of warp-scope are to be accumulated in a thread-safe manner, applied after joining.  
Communication between warps must be done explicitly and carefully,  
 using dedicated communication functions.  
 Communication functions transfer ownership of passed memory.  


Dooty Practice:  

Keep headers as minimal as plausible.  

Inversion of control is evil.  
Assume the programmer is not an ambiguously competent child laborer locked in a cage.  

No exceptions. No exception.  

Allocations should be done as high on the stack as sensible.  
Freeing must always be done within the same scope as its matching allocation.  
Memory ownership transfers may only be done for thread communication.  

Don't use copy or move constructors ever.  
 Containers do construction by being passed a template<E...> for a placement new.  
 Internally, str is the only exception.

Raw memory reallocs of an object must always be valid.  
 Pointer field assignments must not be relative to this.  
 Pointers into a container should never be moved up the scope.  

Pointers to objects in containers are invalidated whenever the container is modified.  
Never use pointers to objects in a container outside of local scope.  

Methods may only modify fields of this, and function ref params.  

Constructors and destructors are fine.  
 Containers call default ctor and dtor.  
 Must obey law of methods.  

Use the suffix "n't" whenever possible  
 lock - locknt  
 allocate - allocatent  
 wait - waitnt  
 This has a tendency to introduce ambiguity  
  firen't:  
   Object is not fire;  
   Object is not becoming fire;  
   Object is not capable of becoming fire;  
   Object does not wish to be associated with fire;  
  The ambiguity is desirable, it prevents complacency.  

Nomenclature  
 add: allocate into a container
 sub: subtract from a container, freeing
 make: construct something at an abstract level, often including an add internally  
 kill: maken't


DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT DOOT DOOT DOOT DOOT HONK DOO DOOT DOOT DOO DOOT  
