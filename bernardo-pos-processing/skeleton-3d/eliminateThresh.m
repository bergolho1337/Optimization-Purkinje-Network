function B = eliminateThresh (A,thresh)
    [l,c,a] = size(A);
    B = zeros(l,c,a);
    for i=1:l,
       for j=1:c,
          for k=1:a,
             if (A(i,j,k) < thresh)
                B(i,j,k) = 0;
             else
                B(i,j,k) = 1; 
             end
          end
       end
    end
end