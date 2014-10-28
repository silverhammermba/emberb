/* raise a LocalJumpError if we don't have a block */
rb_need_block();

/* or the softer approach */
if (rb_block_given_p())
{
	/* code to run when we have a block */
}
