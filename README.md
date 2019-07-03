The Data Oriented Orthodox Template Library is an attempt to remove and wrap the insanity of modern c++, replacing it with data oriented templates and data transform functions.  
All of the STL is strictly removed from headers, this removes approximately [40-400]kLOC from most compilation units, dramatically reducing compilation times, nearer what would be expected of C.  

DOOT is potentially multipurpose, but is intended for gamedev, and is being used as the basis for the unreleased vulkan framework DOOGF  


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

Allocations should be done as high on the stack as sensible.  
Freeing must always be done within the same scope as its matching allocation.  
Memory ownership transfers may only be done for thread communication.  

Raw memory reallocs of an object must always be valid.  
 Pointer field assignments must not be relative to this.  

Pointers to objects in containers are invalidated whenever the container is modified.  
Never use pointers to objects in a container outside of local scope.  

Eschew raw pointers and new/delete,  
in favor of containers and container references.  

Methods may only modify fields of this, and function parameters.  

No copy or move semantics.  
 String is currently a dootnt exception  
Constructors and destructors are fine.  
 Containers call default ctor and dtor.  
 Must obey law of methods.  
No in-place new, use init methods.
 This is undesirable, but placement-new for whatever stupid reason requires STL

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
