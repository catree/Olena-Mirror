    for (unsigned i = 1; i <= nlabels; ++i)
      if (count[i] < 10u)
	data::fill((output | (pw::value(lbl) == pw::cst(i))).rw(), literal::zero);
