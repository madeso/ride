function get_all_factors(number)
    --[[--
    Gets all of the factors of a given number
     
    @Parameter: number
        The number to find the factors of
 
    @Returns: A table of factors of the number
    --]]--
    local factors = {}
    for possible_factor=1, math.sqrt(number), 1 do
        local remainder = number%possible_factor
         
        if remainder == 0 then
            local factor, factor_pair = possible_factor, number/possible_factor
            table.insert(factors, factor)
             
            if factor ~= factor_pair then
                table.insert(factors, factor_pair)
            end
        end
    end
     
    table.sort(factors)
    return factors
end
 
--The Meaning of the Universe is 42. Let's find all of the factors driving the Universe.
 
the_universe = 42
factors_of_the_universe = get_all_factors(the_universe)
 
--Print out each factor
 
print("Count",  "The Factors of Life, the Universe, and Everything")
table.foreach(factors_of_the_universe, print)