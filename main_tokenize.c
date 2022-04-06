int main(void) {
	value *list = tokenize();
	displayTokens(list);
	tfree();
	return 0;
}
