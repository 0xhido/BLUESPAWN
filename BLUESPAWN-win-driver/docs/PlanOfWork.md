## Driver Component PoW

**Part I - Monitoring**

1. Create callbacks capabilities:
   1. Process Creation(/Termination)
   2. Thread Creation(/Termination)
   3. Image Loading
   4. Registry Key Interaction
   5. Process Access (Object Actions)
   6. Create Remote Thread
2. Create Service which will ingest those events
3. Test the communication (Volume, Performance, etc.)

**Part II - Actions**

1. Create termination mechanism at the kernel
2. Create rules for process termination in the service

**Part III - Service Protection**

1. Create kernel level protection to the service

**Part IV - Security**

1. Access token to the driver
2. Cookies

====== Alpha Release ========

**Part V - Add features**

1. Kernel level DLL Injects
2. Kernel level hooking
