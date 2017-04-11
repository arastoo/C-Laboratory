#include	<stdio.h>
#include	<stdlib.h>
#include	<errno.h>
#include	<string.h>
#include	<blkid/blkid.h>


int 
main(int argc, char * argv[])
{
	blkid_probe		pr = NULL;
	int				ret;
	char *	ptname = NULL;

	if(2 != argc) 
	{
		fprintf(stderr, "Program need device path as argument\n");
		exit(EXIT_FAILURE);
	}/* if(2 != argc) */

	/*
	 * This function is same as call open(filename), 
	 * and blkid_probe_set_device(pr, fd, 0, 0).
	 * 
	 * The filename is closed by blkid_free_probe(), 
	 * or by the blkid_probe_set_device() call.
	 *
	 * Returns a pointer to the newly allocated probe 
	 * struct or NULL in case of error.
	 */
	pr = blkid_new_probe_from_filename(argv[1]);
	if(!pr)
	{
		fprintf(stderr, "blkid_new_probe_from_filename failed.\n");
		exit(EXIT_FAILURE);
	}/* if(!pr) */

	/*
	 * Enables/disables the partitions probing 
	 * for non-binary interface.
	 *
	 * pr:		probe
	 * enable:	TRUE/FALSE
	 *
	 * Return 0 on success, or -1 in case of error.
	 */
	if(-1 == blkid_probe_enable_partitions(pr, 1))
	{
		fprintf(stderr, "blkid_probe_enable_partitions failed.\n");
		exit(EXIT_FAILURE);
	}/* if(-1 == blkid_probe_enable_partitions(pr, TRUE)) */

	/*
	 * This function gather probing results from all enabled chains.
	 * Returns 0 on success or 1 in nothing is detected
	 * or -1 in case of error.
	 */
	ret = blkid_do_fullprobe(pr);
	if(1 == ret)
	{
		fprintf(stdout, "blkid_do_fullprobe nothing can detected.\n");
		exit(EXIT_SUCCESS);
	}/* if(1 == ret) */

	if(-1 == ret)
	{
		fprintf(stderr, "blkid_do_fullprobe failed.\n");
		exit(EXIT_FAILURE);
	}/* if(-1 == ret) */

	if(-1 == blkid_probe_has_value(pr, "PART_ENTRY_TYPE"))
	{
		fprintf(stderr, "blkid_probe_has_value failed.\n");
		exit(EXIT_FAILURE);
	}/* if(-1 == blkid_probe_has_value(pr, "PART_ENTRY_TYPE")) */

	if(-1 == blkid_probe_lookup_value(pr, "TYPE",
				(const char **) &ptname, NULL))
	{
		fprintf(stdout, "blkid_probe_lookup_value failed.\n");
		exit(EXIT_FAILURE);
	}/* if(-1 == blkid_probe_lookup_value(pr, "PART_ENTRY_TYPE", */

	printf("%s partition type.\n", ptname);

	blkid_free_probe(pr);

	exit(EXIT_SUCCESS);
}/* main(int argc, char * argv[]) */
