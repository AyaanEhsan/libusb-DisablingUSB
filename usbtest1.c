#include<stdio.h>
#include<stdlib.h>
#include<libusb-1.0/libusb.h>


		int done = 0;
		libusb_device_handle *handle = NULL;

		static int LIBUSB_CALL hotplug_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)

	{
       		 struct libusb_device_descriptor desc;
       		 int rc;

      		  (void)ctx;
   		  (void)dev;
     		  (void)event;
      		  (void)user_data;

      		  rc = libusb_get_device_descriptor(dev, &desc);
       		 if (LIBUSB_SUCCESS != rc) 
			{
               		 fprintf (stderr, "Error getting device descriptor\n");
       			 }
		
			 printf ("Device attached: %04x:%04x\n", desc.idVendor, desc.idProduct);

       			 if (handle)
			 {
        	        libusb_close (handle);
	                handle = NULL;
		        }
	
	       		rc = libusb_open (dev, &handle);
		        if (LIBUSB_SUCCESS != rc) {
	                fprintf (stderr, "Error opening device\n");
	       		 }

    				  done++;

      				  return 0;
		}




		static int LIBUSB_CALL hotplug_callback_detach(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event event, void *user_data)

	{
       		 (void)ctx;
      		 (void)dev;
      		 (void)event;
       		 (void)user_data;

       		 printf ("Device detached\n");

       		 if (handle)
		 {
                libusb_close (handle);
                handle = NULL;
       		 }

       		 done++;

       		 return 0;
	}












        void print_devices(libusb_device *dev)
{
        struct libusb_device_descriptor desc;
        struct libusb_config_descriptor *config;
        const struct libusb_interface *inter;
        const struct libusb_interface_descriptor *interdesc;
        const struct libusb_endpoint_descriptor *endpointdesc;

	struct libusb_device_handle *handle=NULL;	

        int ret;
        int i,j,k;

        ret=libusb_get_device_descriptor(dev,&desc);

        if(ret<0)
        {
        fprintf(stderr,"error in detting device descriptor\n");
        return;
        }

        printf("Number of POS configs is %d \n",desc.bNumConfigurations);
        printf("Device Vendor id):%d \n",desc.idVendor);
        printf("Product ID: %d \n",desc.idProduct);
	printf("Class ID: %d \n",desc.bDeviceClass);
        libusb_get_config_descriptor(dev,0,&config);
        printf("Interface: %d \n", config->bNumInterfaces);

	if(desc.idVendor==34148)
	{
		libusb_open(dev,&handle);
		libusb_detach_kernel_driver(handle,0);
		libusb_close(handle);
	}

            /* -----------------------------------------------------------
                        in the above if , include the Devices Vendor Id to block 
                        the Device.
               -----------------------------------------------------------*/
        printf("\n \n");
}




int main(int argc,char *argv[])
{
        libusb_hotplug_callback_handle hp[2];
        int product_id, vendor_id, class_id;
        int rc;

	libusb_device **devs;
	libusb_context *context = NULL;

	size_t list;
	size_t i;
	int ret;


        rc = libusb_init (&context);
	        if (rc < 0)
       		 {
               		 printf("failed to initialise libusb: %s\n", libusb_error_name(rc));
               		 return EXIT_FAILURE;
       		 }


		
		if (!libusb_has_capability (LIBUSB_CAP_HAS_HOTPLUG))
		 {
                printf ("Hotplug capabilites are not supported on this platform\n");
                libusb_exit (NULL);
                return EXIT_FAILURE;
       		 }





	list=libusb_get_device_list(context,&devs);

if(list<0)
{
fprintf(stderr,"Error in getting the Device list\n");
libusb_free_device_list(devs,1);
libusb_exit(context);
exit(1);
}

        printf("There are %d Devices Found\n",list);

for(i=0;i<list;i++)
{
 //Prints device Specifications
        print_devices(devs[i]);
}

	printf("\n Device with Vendor ID :- 34148 has been Detached \n");

libusb_free_device_list(devs,1);
libusb_exit(context);
  return 0;

}
