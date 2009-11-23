<?php

$data = file_get_contents("edinburgh.svg");
$r = new RsvgHandle($data);
$s = new CairoImageSurface(CairoFormat::ARGB32, 1000, 1000);
$c = new CairoContext($s);
$r->renderCairo($c);
$s->writeToPng("edinburgh.png");
